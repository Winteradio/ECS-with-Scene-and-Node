# Entity Component System

진행 기간: 2023/03/01 → 2023/08/13
스킬: C++, CMake, ECS

Github [ECS Project](https://github.com/Winteradio/ECS-with-Scene-and-Node)

Github [Log Project](https://github.com/Winteradio/Log)

Github [Memory Project](https://github.com/Winteradio/MemoryPool)

Github [System Dependency](https://github.com/Winteradio/System-Dependency)

---

## 요약

- ECS ( Entity Component System ) 재구성
- 부수적인 라이브러리 생성
    - Log Project
    - Memory Project

## 🛠️ 기술 스택 및 라이브러리

- 언어
    - C++
- 라이브러리
    - Log Project
    - Memory Project
    - Rpc ( UUID )

## 📝 개념 설명

### ECS ( Entity Component System ) 이란?

- 유니티( Unity )에서 개발 중인, DOTS 기반 시스템
- DOTS vs OOP
    - DOTS 란 ?
        - 데이터 지향 기술 스택 ( Data-Oriented Technology Stack )
        - 데이터 중심적인 프로그래밍 접근 방식을 의미
    - OOP와의 차이점은 무엇인가?
        - 데이터 중심적
            - 성능 최적화 및 데이터 자체에 대한 접근 방식
        - 연속적인 메모리 접근성
            - OOP의 객체와 달리, 동일한 타입의 데이터들을 연속된 메모리에 저장
            - 메모리 접근 패턴을 최적화하여 캐시 히트 ( Cache Hit )의 확률을 높임
            - 물량 시뮬레이션과 같이 동일한 타입의 데이터들이 다량으로 존재하는
            부분에서 보다 더 높은 성능을 나타냄
        - 병럴 처리의 용이성
            - 데이터 타입별로 접근할 수 있기에, 
            사용되는 메소드( or System )들을 독립적으로 구성할 수 있음
            - 메소드( or System )들은 객체에 대한 접근이 아닌 데이터에 대한 접근이기에,
            객체( Object, Entity )와 시스템( System )의 분리가 가능함
- Entity Component System
    - Entity ( 객체 혹은 네이밍 ) 란?
        - 게임 오브젝트, 캐릭터 등과 같은 개별적인 객체를 나타내는 추상화
        - 단순 식별자 역할 ( 네이밍 )을 하며 데이터를 직접 지니지 않음
        - 예시
            - Entity ID : “홍길동”
                - “능력” Component - “홍길동”
                - “스킬” Component - “홍길동”
                - “무기” Component - “홍길동”
            - 위와 같은 Entity는 Component들의 식별자 역할
    - Component ( 구성 요소 ) 란?
        - 실제 사용되는 데이터를 나타냄
        - Entity에 부여될 시, Entity의 ID로 Component를 식별할 수 있음
        - Component타입에 따라 연속된 메모리상에 존재해야 함
    - System ( 시스템 ) 이란?
        - Component를 처리하고 조작하는 로직
        - 각각의 System은 필요로하는 Component에 접근 권한을 지님
            - Entity가 직접 System과 연결되는 것이 아닌, ID를 통한 Component로의 접근
        - 각각의 System을 병렬로 처리할 수도 있지만, 
        서로 간의 의존성을 파악한 후, 독립적인 시스템들간의 병렬성을 추구해야 함
        - 예시
            - System : “Render”
                - 사용 Component : “Render Component”
            - System : “Move”
                - 사용 Component : “Move Component”

### ECS의 종류 : Sparse Set vs Archetype

- Sparse Set
    - Entity ID와 각각의 Component을 연결시키는 방법
    - Entity ID을 저장하는 Dense Set과 Component들을 저장하는 Sparse Set 사용
    - 해당 Entity ID를 활용하여서 연속된 메모리 상에 존재하는
    Component에 빠르게 접근하는 방식
    - 관련 사이트 : [Github의 Entt 라이브러리](https://github.com/skypjack/entt)
- Archetype
    - 비슷한 Component 구성을 가진 Entity들을 다루는 그룹
    - 각각의 Archetype은 고유한 Component 구성을 지님
    - Sparse Set과 달리, Component들을 연속적인 데이터로 저장하지 않고,
    동일한 타입의 Archetype들을 연속된 메모리 상에 저장시킴
    - 관련 사이트 : [Unity의 ECS Archetype](https://docs.unity3d.com/Packages/com.unity.entities@1.0/manual/concepts-archetypes.html)

### 왜 ECS를 OOP 언어인 C++에서 구현하려는 것인가?

- 게임 엔진 구조에 대한 이해
    - 지속적으로 사용할 수 있는 라이브러리 필요
    - 엔진의 내부적인 동작 원리에 대한 이해도 부족
        - 프로젝트 파일은 어떻게 구성되어있는지
        - 엔진 상에서의 플레이, 퍼즈는 어떤 식으로 동작하는지
        - 엔진 상에서 게임 빌드는 어떻게 이뤄지는지
            - C++ 상에서 런타임 중에 해당 프로그램을 재빌드하는 것이 불가능
            - 그렇다면 엔진과 게임을 따로 나눠서 진행하는건지
    - 다른 엔진들의 UML 정보만으론 구현하기 어려움
- 독립적인 시스템들의 구현
    - 충돌, 유체 시뮬레이션과 같은 하나의 기능을 구현하기 위한 밑작업이 필요했음

- 조금 다른 ECS를 만들어 보고 싶었음
    - Sparse Set의 문제점
        - Entity ID를 통해서 오직 하나의 Component에 대한 접근만 가능함
        - 모든 System들이 하나의 Component만을 사용할 수는 없음
    - Archetype의 문제점
        - 특정 Archetype과 한,두개의 Component만 추가하고 싶더라도
        새로운 Archetype으로 데이터들을 복제시켜서 만들어야 했음
        - 예시
            - 빛이 나면서 렌더링이 되는 객체를 만들고 싶을 때
            - Render Archetype
                - Mesh Component
                - Transform Component
            - Light Archetype
                - Color Component
                - Transform Component
            - 이 경우에는, Light Render Archetype을 새로 선언
            필요로 하는 Component들을 복제 후,
            기존의 Archetype에서 삭제시켜야 했음
    - Node 개념을 활용
        - Unity나 Unreal Engine에서 자주 등장하는 Node개념을 사용
        - 각각의 Component들에 접근권한이 있는 Node개념을 추가하게 된다면
        Sparse Set과 Archetype의 개념을 같이 사용할 수 있다고 생각
            - Entity ID로 Component에 접근하는 것을 매 프레임마다 하는것이 아닌,
            Node가 생성될 때 해당 Component들에 대한 접근 부여
                - **Entity에 Node 등록할 때 : Sparse Set**
                - **각 Node는 지닐 수 있는 Component 조합이 고유함 : Archetype**
        - 예시
            - 빛이 나는 렌더링이 되는 객체를 만들고 싶을 때
            - Render Node
                - Mesh Component
                - Transform Component
        
        - Light Node
            - Color Component
            - Transform Component
        - 이 경우엔 Archetype처럼 새로운 Node를 추가하는 것이 아닌,
        동일한 Entity에 Render, Light Node를 등록시켜서 사용 가능
            - 이렇게 될 시, 동일한 Transform Component를 사용
            - 물론, 동일한 데이터을 변환시키는 서로 다른 시스템이 있을 시,
            시스템을 재구성하거나 변환에 제재를 주는 식으로 구현
        - 즉, Entity가 다양한 Node들을 가질 수 있음
        - Node를 활용하게 될 경우, 기존의 Entity를 유지하면서 
        다양한 Component들을 해당 Entity에 추가 및 삭제가 가능

## 📝 개발 내용

### 인터페이스

- IObject
    - IComponent
    - INode
    - ISystem
    - ( Entity )

### 클래스

- Scene
- Entity ( Parent : IObject )

### 매니저

- SceneManager
- EntityManager
- ComponentManager
- SystemManager
- NodeManager

### 부수

- MyUUID

## 📝 상세 개발 내용

### 인터페이스

- IObject
    - MyUUID를 사용하는 객체들을 위한 인터페이스
        
        ```cpp
        class IObject
        {
            public :
                IObject() {};
                IObject( MyUUID ID ) : m_ID( ID ) {};
                virtual ~IObject() {};
        
            public :
                void SetID( MyUUID ID ) { m_ID = ID; }
                MyUUID GetID() { return m_ID; }
                MyUUID GetID() const { return m_ID; }
        
            protected :
                MyUUID m_ID;
        };
        ```
        

- IComponent
    - ComponentManager에서 IComponent 포인터로 객체 주소를 저장하기 위한 인터페이스
        
        ```cpp
        class IComponent : public IObject
        {
            public :
                IComponent() {};
                IComponent( MyUUID ID ) : IObject( ID ) {};
                virtual ~IComponent(){};
        };
        ```
        

- INode
    - NodeManager에서 INode 포인토로 객체 주소를 저장하기 위한 인터페이스
        
        ```cpp
        class INode : public IObject
        {
            public :
                INode() {};
                INode( MyUUID ID ) : IObject( ID ) {}
                virtual ~INode(){};
        
            public :
                virtual bool Check( MemoryPtr<Entity>& EntityMPtr ) = 0;
                virtual void Init( MemoryPtr<Entity>& EntityMPtr ) = 0;
        };
        ```
        
        - Check( MemoryPtr<Entity>& EntityMPtr )
            - 필요로 하는 Component들을 NodeManager를 통해서 Entity가 지녔는지 확인
        - Init( MemoryPtr<Entity>& EntityMPtr )
            - Component들을 지녔을 시, ComponentManager와 Entity를 통해서
            필요로 하는 Component들의 MemorPtr을 획득
    - 각 Node들은 Entity와 동일한 ID를 지님
        - 해당 Entity에 대해 중복되는 Node가 있을 수 없기 때문
- ISystem
    - SystemManager에서 ISystem 포인터로 객체 주소를 저장하기 위한 인터페이스
        
        ```cpp
        class ISystem : public IObject
        {
            public :
                ISystem() {};
                ISystem( MyUUID ID ) : IObject( ID ) {};
                virtual ~ISystem(){};
        
            public :
                virtual void Update( float DeltaTime, int ScenIndex, MyUUIDUnSet& EntityID ) = 0;
        
                virtual void SetNodeType( const std::type_info* Type ) = 0;
                const std::type_info*& GetNodeType() { return m_NodeType; }
        
            protected :
                const std::type_info* m_NodeType;
        };
        ```
        
        - m_NodeType
            - 해당 System에서 요구하는 Node 타입의 정보
            - Scene의 Update 함수에서 System에서 요구하는 Node 타입 정보를 
            토대로 MyUUID Set 데이터를 주기 때문
        - Update( float DeltaTime, int SceneIndex, MyUUIDUnSet& EntityID ) = 0;
            - SceneIndex
                - 해당 Scene의 정보가 있어야 System상에서 
                Node, Component, Entity 등을 추가할 수 있는 기능을 넣을 수 있음

### 클래스

- Scene
    - 주된 역할
        - Unity의 ECS 구조에서는 World에 해당하는 역할
        - 실제 Entity ID( = Node ID )를 다루며, System들을 실행시키는 객체
        - 실제 Update( Frame )을 다루는 객체
    - Node 관련
        - Node ID ( = Entity ID )을 사용하여 등록
        - 각 Node 타입별로 ID들을 저장
            - 새로운 Node 타입이 등록 될시, **유효성** 검사 실시
            - System에서 요구하는 Node들의 ID Set 전달하기 위함
    - System 관련
        - System ID을 사용하여 등록
        - 의존성 검사
            - 등록 시, SystemManager에서 의존성 검사 실시
            - Scene에 등록되어 있는 **유효한** System 간의 Sequence 확인
        - 유효성 검사
            - 등록 시, 현재 Scene에서 필요로 하는 Node 타입이 있는지 확인
            - 있을 시, **유효한** System ID로 변경
        - Update 실행
            - **유효한** System Sequence들을 실행
    - **유효성 검사**
        - Node를 통한, 등록된 System의 사용가능 여부
            
            ```cpp
            // System에 대한 유효성 검사
            void Scene::CheckSystemValidity( const std::type_info* Type )
            {
                auto NodeITR = m_NodeTypeData.find( Type );
                if ( NodeITR == m_NodeTypeData.end() ) return ;
            
                auto SystemITR = m_PendingSystemIDData.find( Type );
                if ( SystemITR == m_PendingSystemIDData.end() ) return;
            
                for ( auto SystemID : m_PendingSystemIDData[ Type ] )
                {
                    m_SystemIDData.insert( SystemID );
                }
            
                m_PendingSystemIDData.erase( SystemITR );
            
                UpdateSequence();
            }
            
            // 유효성 검사 후 SceneManager를 통한 의존성 검사
            void Scene::UpdateSequence()
            {
                SystemManager::GetHandle().UpdateSequence( m_SystemIDData, m_MainSequence );
                ClearSequence( m_DoneSequence );
            }
            
            // Node 추가 시, 유효성 검사 실시
            void Scene::RegisterNode( const std::type_info* Type, MyUUID ID )
            {
                bool Check = HasNode( Type, ID );
            
                if ( !Check ) m_NodeIDData[ Type ].insert( ID );
            }
            
            bool Scene::HasNode( const std::type_info* Type, MyUUID ID )
            {
                {
                    auto ITR = m_NodeTypeData.find( Type );
                    if ( ITR == m_NodeTypeData.end() )
                    {
                        m_NodeTypeData.insert( Type );
                        m_NodeIDData[ Type ] = MyUUIDUnSet();
            
                        CheckSystemValidity( Type );
                        return false;
                    }
                }
            
                {
                    auto ITR = m_NodeIDData[ Type ].find( ID );
                    if ( ITR != m_NodeIDData[ Type ].end() ) return true;
                    else return false;
                }
            }
            
            // System 추가 시, 유효성 검사 실
            void Scene::RegisterSystem( const std::type_info* Type, MyUUID ID )
            {
                bool Check = HasSystem( ID );
            
                if ( !Check )
                {
                    auto ITR = m_PendingSystemIDData.find( Type );
                    if ( ITR == m_PendingSystemIDData.end() )
                    {
                        m_PendingSystemIDData[ Type ] = MyUUIDUnSet();
                    }
            
                    m_PendingSystemIDData[ Type ].insert( ID );
                    CheckSystemValidity( Type );
                }
            }
            ```
            
            - Scene::CheckSystemValidity( const std::type_info* Type )
                - 해당 Type에 대한 유효성 검사 실시
                - m_NodeTypeData
                    - 현재 Scene이 지닌 NodeType에 대한 정보
                - m_PendingSystemIDData
                    - 해당 NodeType을 요구하는 유효성 검사를 통과하지 
                    못한 System들의 ID Data
                - m_SystemIDData
                    - 유효성 검사를 통과한 System들의 ID Data
                - m_NodeTypeData과 m_PendingSystmeIDData에 Type이 있는지 확인
                - 있다면, m_SystemIDData로 해당 SystemID를 넘겨줌
                - Node에 새로운 타입이 추가되거나, 새로운 System이 추가될 때마다 호출

- Entity
    - 주된 역할
        - Component들을 등록 및 확인하는 객체
    - Component 관련
        - Component ID을 사용하여 등록
        - 각 Component 타입별로 유일한 ID를 등록

### 매니저

- SceneManager
    - 주된 역할
        - Scene을 생성 및 삭제하는 역할
    - Scene 관련
        - Scene을 생성
            - Scene에 Index 부여
            - Scene의 경우에는 인스턴스가 많이 생성되지 않기에, UUID는 불필요하다고 생각
        - Index를 통해 Scene에 접근 가능
        - 전체 Index 정보를 가져올 수 있음
            - 파일 저장 및 불러오기, UI에 사용하기 위해서
- EntityManager
    - 주된 역할
        - Entity를 생성 및 삭제하는 역
    - Entity 관련
        - Entity를 생성
        - ID를 통해 Entity에 접근 가능
        - 전체 ID 정보를 가져올 수 있음
            - 파일 저장 및 불러오기, UI에서 사용하기 위해서
- ComponentManager
    - 주된 역할
        - Component 생성 및 삭제하는 역할
    - Component 관련
        - Component를 생성
        - ID와 Type 정보를 통해 Component에 접근 가능
        - 전체 Type 정보를 가져올 수 있음
            - 해당 Type에 전체 ID 정보를 가져올  수 있음
                - 파일 저장 및 불러오기, UI에서 사용하기 위해서
- SystemManager
    - 주된 역할
        - System 생성 및 삭제하는 역할
    - System 관련
        - System을 생성
        - ID를 통해  System에 접근 가능
        - 전체 ID 정보를 가져올 수 있음
        - 의존성 설정 가능
            
            ```cpp
            // 의존성 설정
            void SystemManager::SetDependency( MyUUID MainID, MyUUID DependencyID )
            {
                bool Check = HasSystem( MainID ) && HasSystem( DependencyID );
            
                if ( !Check ) return;
            
                m_DependencyID[ MainID ].insert( DependencyID );
            
                MyUUIDUnSet TestIDSet;
            
                for ( auto [ ID, System ] : m_ISystemUnMap )
                {
                    TestIDSet.insert( ID );
                }
            
                Check = TopologySort( TestIDSet );
            
                if ( !Check ) { DeleteDependency( MainID, DependencyID ); }
            }
            
            // 의존성 삭제
            void SystemManager::DeleteDependency( MyUUID MainID, MyUUID DependencyID )
            {
                bool Check = HasSystem( MainID );
            
                if ( Check )
                {
                    auto ITR = m_DependencyID[ MainID ].find( DependencyID );
            
                    if ( ITR == m_DependencyID[ MainID ].end() )
                    {
                        Log::Warn( " %s System has not %s System ", typeid( *GetSystem( MainID ) ).name(), typeid( *GetSystem( DependencyID) ).name() );
                        return;
                    }
                    else
                    {
                        Log::Info( " Erase Dependency %s for %s ", typeid( *GetSystem( MainID ) ).name(), typeid( *GetSystem( DependencyID) ).name() );
                        m_DependencyID[ MainID ].erase( ITR );
                    }
                }
            }
            ```
            
            - SetDependency( MyUUID MainID, MyUUID DependencyID )
                - MainID : 의존되어지는 System ID
                - DependencyID : 의존하는 System ID
                - 두 ID를 SystemManager에서 보유 중인지 확인
                    - 보유 중일 시, Dependency 기입
                    - m_DependencyID : 해당 ID를 의존하는 ID Set들의 모임
                - 순환 그래프 여부를 확인하기 위한 Test용 IDSet 객체 생성
                    - 순환 그래프일 시, 해당 Dependency 삭제
            - DeleteDependency( MyUUID MainID, MyUUID DependencyID )
                - MainID : 의존되어지는 System ID
                - Dependency : 의존하는 System ID
                - MainID가 SystemManager에서 보유 중인지 확인
                    - 보유 중일 시, 해당 MainID에서 DependencyID의 의존성 제거
        - 순환 그래프 여부 확인
        - 
        
        ```cpp
        
        ```
        
        - 전체 System간의 의존성 정보를 가져올 수 있음
- NodeManager
    - 주된 역할
        - Node 생성 및 삭제하는 역할
    - Node 관련
        - Entity를 통해서 Node를 생성
            
            ```cpp
            template< typename T >
                    MemoryPtr<T> NodeManager::Create( MemoryPtr<Entity>& Object )
                    {
                        bool Check = HasIDSet<T>();
                        if ( !Check ) CreateIDSet<T>();
            
                        MemoryPtr<T> NodeMPtr = MemoryManager::GetHandle().Create<T>( Object->GetID() );
                        
                        Check = NodeMPtr->Check( Object );
            
                        if ( !Check ) 
                        { 
                            MemoryManager::GetHandle().Delete<T>( NodeMPtr );
                            throw Except( " NodeManager | %s | This Entity has not components for this node ", __FUNCTION__, typeid( T ).name() );
                        }
            
                        NodeMPtr->Init( Object );
                        GetIDData<T>().insert( NodeMPtr->GetID() );
                        m_INodeMPtrUnMap[ NodeMPtr->GetID() ] = NodeMPtr;
            
                        return NodeMPtr;
                    }
            ```
            
            - 주어진 Entity를 통해서 Node 생성
            - Node에서 요구하는 Component 확인
                - Check : false 일시
                    - 해당 Node를 다시 삭제
                - Check : true 일시
                    - NodeData에 기입
        - ID와 Type을 통해 Node에 접근 가능
        - 전체 Type 정보를 가져올 수 있음
            - 해당 Type에 전체 ID 정보를 가져올 수 있음
                - 파일 저장 및 불러오기, UI에서 사용하기 위해서

## 💻 ECS 예시 코드

### 순환적 System의 구성

### Render Node를 만들고 싶지만, 필요로 하는 Component들이 없네

### Physics Node가 만들어질 때까지 기다리자

## 💡 성장 경험

### 게임 엔진 내부 구성에 대한 이해

- 아직 실제 게임 엔진을 제대로 다 만들어 보진 않았지만, 
내부 객체들의 구성을 어떤 식으로 만들어야 하는지에 대한 기초적인 이해가 된 것 같다
- 예전에 만들었던 코드들의 경우에는 기능이나 클래스를 추가하려고 하면, 
어떤 부분을 어떻게 만들어야 하는지에 대한 느낌이 없었지만, 
이 프로젝트를 진행하면서 앞으로 System을 추가하거나, Node 혹은 Component를 추가할 때,
혹은 Manager에서 기능을 추가 및 보완할 때 어떻게 해야할 지에 대한 
방향성은 주어졌기 때문에 보다 나은 설계를 할 수 있을 것 같다.
- MemoryProject나 LogProject도 예전 같았다면 단순히 소스파일을 해당 프로젝트에 
추가해서 사용했을텐데, 실제 엔진에서 사용하는 것처럼 만들고 싶어서, 
정적이나 동적 라이브러리를 만들어서 사용하였다.
추후에 ECS나 ECS를 사용한 게임엔진을 사용해서, 무언가를 만들 때에도 라이브러리를 
CMake로 추가하여서 설정한다면 굉장히 편하게 사용할 수 있을 것 같다.

### 객체에 대한 고찰

- 이번에 여러개의 인터페이스를 만들면서, OOP에서 객체와 상속에 대해서 다시 공부하게 되었다.
- 원래 NodeManager와 ComponentManager의 경우에는 void*나 std::any를 사용하여서
각 타입별 MemoryPtr을 저장하였었지만, 조금 더 다르게 사용하고 싶었다.
- Node와 Component에 해당하는 INode, IComponent 인터페이스를 만들어서,
그 인터페이스와 현재 만들려고 하는 객체 간의 상속의 연관성으로 생성의 성공 혹은 실패를
dynamic_cast를 통해서 구현하고자 하였고, 이때 RTTI와 vtable에 대해 조금 더 알게 되었다.
- 물론 RTTI를 남용한다면 오버헤드가 발생하여서 성능적으로 저하가 발생한다고 하지만,
지금 수준에선 크게 생각할 부분이 아니라고 생각되었다.

### 알고리즘의 중요성

- 위상 정렬 ( Topology Sort )를 System Manager에서 구현하여서 의존성을 테스트하기 전에는
단순히 연결리스트에 System들을 배치시키고, 사용자가 직접 전체 의존성을 설정하도록 하였다.
- 처음에 생각했을 때에는 System들이 RenderNode, MoveNode, CollisionNode들 밖에 
생각이 안 났기에 괜찮겠다라고 생각이 들었다.
- 하지만, 위상 정렬이 게임 안에서 생각보다 다양하게 많이 쓰인다는 걸 알게 되었다.
예를 들면, 의존성이 있는 퀘스트나 특정한 직업으로 전직하기 위한 의존성 검사와 같은 것들을
알게 되었고, 지금 단순한 System의 의존성조차 손으로 직접 구현하지 못한다면 문제가 생길 것 같았다.
- 실제로, System의 의존성에 대한 DAG ( 비순환 그래프 ) 검사 후, 위상 정렬을 구현해서
System Sequence를 확인해보니, 연결리스트를 만들 때보다 매우 확실히 편리하였다. 
또한, 이때에도, 그렇다면 System 전체가 의존성을 설정한 후에 검사를 해야하나 아니면
의존성이 설정될 때마다 해야될까 등등의 고민을 하면서 보다 나은 선택을 하려고 하였다.
- System 전체의 의존성이 결정된 후에 검사를 하게 될 시, 만약 순환 그래프가 생기게 된다면
어떤 System을 제거할지 명확하지 않았다. 
그렇기에, 매 System이 의존성을 설정할 때마다 DAG ( 비순환 그래프 ) 인지 검사하며, 
순환 그래프일 시, 현재 등록하려는 System의 의존성을 제거하는 방식을 추구하였다.