#ECS Framework: C++용 데이터 지향 엔티티 컴포넌트 시스템
##소개Custom ECS Framework는 C++17 환경에서 사용할 수 있도록 설계된 **데이터 지향(Data-Oriented) 엔티티 컴포넌트 시스템(ECS)**입니다.
이 시스템은 게임 로직과 데이터를 분리하여 유연성을 확보하고, **Node 시스템**을 통해 필요한 데이터에만 효율적으로 접근하는 것을 목표로 합니다. 
또한, 시스템 간의 실행 순서를 자동으로 관리하는 **의존성 그래프(Dependency Graph)** 기능을 제공하며, 
`MemoryProject`와 연동하여 자동화된 **가비지 컬렉션(GC)**을 지원합니다.

##주요 기능* **Node 기반 데이터 접근:**
* 단순히 엔티티를 순회하는 것이 아니라, `Node`라는 개념을 사용하여 특정 컴포넌트 조합을 가진 엔티티만을 필터링합니다.
* 시스템은 `NodeContainer`를 통해 자신에게 필요한 데이터(컴포넌트 묶음)에 직관적으로 접근합니다.


* **시스템 의존성 그래프 (Dependency Graph):**
* `DependOn` 메서드를 사용하여 시스템 간의 의존 관계를 정의하면, 프레임워크가 위상 정렬(Topological Sort)을 수행하여 올바른 실행 순서를 자동으로 계산합니다.
* 순환 의존성(Circular Dependency) 발생 시 이를 감지하여 안정성을 보장합니다.


* **리플렉션 기반 가비지 컬렉션:**
* `MemoryProject`의 스마트 포인터(`Memory::ObjectPtr`) 및 리플렉션 시스템과 통합되어 있습니다.
* `PROPERTY` 매크로를 통해 등록된 멤버 변수는 GC에 의해 자동으로 수명(Reachability)이 관리됩니다.


* **유연한 레지스트리 시스템:**
* `Entity`, `Component`, `Node`, `System` 각각을 위한 전용 레지스트리(`Registry`)를 제공하여 객체의 생성, 검색, 삭제를 효율적으로 처리합니다.



##[중요] 가비지 컬렉션 주의사항 (PROPERTY 매크로)이 프레임워크는 **리플렉션 기반의 가비지 컬렉터(GC)**를 사용합니다. GC는 `Memory::RootPtr`에서 시작하여 도달 가능한(Reachable) 객체를 추적합니다.

* **필수 사항:** `World` 클래스나 컨테이너 내에서 `EntityContainer`, `SystemRegistry` 등의 멤버 변수를 선언할 때, 반드시 **`PROPERTY()` 매크로를 사용하여 리플렉션 시스템에 등록해야 합니다.**
* **경고:** `PROPERTY()` 매크로를 누락할 경우, GC는 해당 멤버 변수를 추적할 수 없습니다. 결과적으로 **참조 중인 데이터가 도달 불가능(Unreachable)하다고 판단되어 `Memory::Collect()` 실행 시 메모리에서 강제로 해제**되며, 이는 심각한 오류(Crash)로 이어질 수 있습니다.

```cpp
struct World
{
    GENERATE(World); // 리플렉션 정보 생성

public:
    // [올바른 예] GC가 entity 컨테이너를 추적하여 내부 데이터를 보존함
    PROPERTY(entity);
    ECS::EntityContainer entity;

    // [잘못된 예] GC가 이 변수를 볼 수 없음 -> GC 실행 시 데이터 삭제됨!
    // ECS::NodeRegistry node; 
};

```

##핵심 컴포넌트| 컴포넌트 | 역할 | 관련 파일 |
| --- | --- | --- |
| **World** | Entity, Component, System 등 모든 레지스트리를 총괄하는 루트 객체입니다. | `demofile/main.cpp` (예시 참조) |
| **Node** | 시스템이 처리할 컴포넌트들의 조합(필터)을 정의합니다. | `demofile/NodeList.h`, `include/Registry/NodeRegistry.h` |
| **System** | 실제 로직을 수행하며, 의존성 그래프를 통해 실행 순서가 결정됩니다. | `include/Object/System.h`, `include/Registry/SystemRegistry.h` |
| **Component** | 로직이 없는 순수 데이터 컨테이너입니다. | `include/Container/ComponentContainer.h` |
| **Graph** | 시스템 간의 의존성(`DependOn`)을 분석하고 실행 순서를 정렬합니다. | `include/Graph/Graph.h` |

##사용 예시```cpp
#include <ECS.h>
#include "ComponentList.h" // 컴포넌트 정의 헤더
#include "NodeList.h"      // 노드 정의 헤더
#include "SystemList.h"    // 시스템 정의 헤더

// 1. World 정의 (GC 루트 역할을 함)
struct World
{
    GENERATE(World); // 리플렉션 필수

public:
    // PROPERTY 매크로를 통해 GC에 멤버 등록 (매우 중요)
    PROPERTY(entity);
    ECS::EntityContainer entity;

    PROPERTY(component);
    ECS::ComponentRegistry component;

    PROPERTY(node);
    ECS::NodeRegistry node;

    PROPERTY(system);
    ECS::SystemRegistry system;

    PROPERTY(scene);
    ECS::Scene scene;

    void Update(const ECS::TimeStep& timeStep)
    {
        // 시스템 의존성 그래프 빌드 및 정렬된 실행 순서 획득
        const auto graph = system.BuildGraph();
        const auto sortedIDs = graph.GetPartialSorted(scene.GetSystemID());

        // 순서대로 시스템 실행
        for (const auto& systemID : sortedIDs)
        {
            auto sys = system.Get(systemID);
            if (sys)
            {
                // 해당 시스템이 필요로 하는 Node 컨테이너만 전달
                auto nodeContainer = node.GetContainer(sys->GetNodeType());
                sys->Update(timeStep, nodeContainer);
            }
        }
    }
};

void main()
{
    // 2. 메모리 및 World 초기화
    Memory::Init(1024 * 1024, 10000);
    Memory::RootPtr<World> world = Memory::MakePtr<World>();

    // 3. 엔티티 생성
    auto player = world->entity.Emplace("Player1");

    // 4. 컴포넌트 부착 (데이터 설정)
    world->component.Create<TransformComponent>(player->GetID());
    world->component.Create<VelocityComponent>(player->GetID());

    // 5. 노드 생성 (엔티티와 컴포넌트 연결)
    // 시스템은 이 'PhysicsNode'를 통해 데이터에 접근합니다.
    auto physicsNode = world->node.Create<PhysicsNode>(player->GetID());

    // 6. 시스템 생성 및 의존성 설정
    auto sMove = world->system.Create<MoveSystem>();
    auto sRender = world->system.Create<RenderSystem>();

    // 렌더링은 이동 로직이 끝난 후에 실행되어야 함
    sRender->DependOn(sMove);

    // 7. 씬(Scene)에 등록
    world->scene.RegisterNode(physicsNode);
    world->scene.RegisterSystem(sMove);
    world->scene.RegisterSystem(sRender);

    // 8. 업데이트 루프 실행
    ECS::TimeStep timeStep;
    world->Update(timeStep);

    // 9. 가비지 컬렉션 및 해제
    Memory::Collect();
    Memory::Release();
}

```