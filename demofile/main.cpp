#include <Log/include/Log.h>
#include <Log/include/LogPlatform.h>

#include <Memory/include/Memory.h>

#include <ECS.h>

#include "ComponentList.h"
#include "NodeList.h"
#include "SystemList.h"

struct World
{
	GENERATE(World);

public :
	PROPERTY(entity);
	ECS::EntityContainer entity;

	PROPERTY(node);
	ECS::NodeRegistry node;

	PROPERTY(component);
	ECS::ComponentRegistry component;

	PROPERTY(system);
	ECS::SystemRegistry system;

	PROPERTY(scene);
	ECS::Scene scene;

	METHOD(Update);
	void Update(const ECS::TimeStep& timeStep)
	{
		LOGINFO() << "[TEST] World Update - Start";

		const ECS::SystemRegistry::GraphType graph = system.BuildGraph();
		const wtr::DynamicArray<ECS::UUID> partialID = graph.GetPartialSorted(scene.GetSystemID());

		for (const auto& systemID : partialID)
		{
			auto sSystem = system.Get(systemID);
			if (!sSystem)
			{
				continue;
			}

			const auto nodeType = sSystem->GetNodeType();
			auto nodeContainer = node.GetContainer(nodeType);

			sSystem->Update(timeStep, nodeContainer);
		}

		LOGINFO() << "[TEST] World Update - Done";
	}
};

void Test()
{
	LOGINFO() << "[TEST] Make the registry data";

	// Create the world and register the GC.
	Memory::RootPtr<World> world = Memory::MakePtr<World>();

	// Create the entity.
	Memory::ObjectPtr<ECS::Entity> entity = world->entity.Emplace("Hello");

	// Create the component with entity's id or arguments.
	Memory::ObjectPtr<MeshComponent> cMesh = world->component.Create<MeshComponent>(entity->GetID());
	Memory::ObjectPtr<VelocityComponent> cVelocity = world->component.Create<VelocityComponent>(entity->GetID());
	Memory::ObjectPtr<TransformComponent> cTransform = world->component.Create<TransformComponent>(entity->GetID());
	Memory::ObjectPtr<ColorComponent> cColor = world->component.Create<ColorComponent>(entity->GetID(), 1.0, 0.0, 0.0, 1.0);
	Memory::ObjectPtr<CameraComponent> cCamera = world->component.Create<CameraComponent>(entity->GetID(), 45.0, 1.0, 1000.0, 1080, 900);
	Memory::ObjectPtr<ResourceComponent> cResource = world->component.Create<ResourceComponent>(entity->GetID(), "Basic");

	// Create the node with entity's id or arguments.
	Memory::ObjectPtr<RenderNode> nRender = world->node.Create<RenderNode>(entity->GetID(), cResource, cTransform, cColor);
	Memory::ObjectPtr<CameraNode> nCamera = world->node.Create<CameraNode>(entity->GetID());
	Memory::ObjectPtr<CollisionNode> nCollision = world->node.Create<CollisionNode>(entity->GetID());
	Memory::ObjectPtr<PhysicsNode> nPhysics = world->node.Create<PhysicsNode>(entity->GetID());

	// Create the system.
	Memory::ObjectPtr<RenderSystem> sRender = world->system.Create<RenderSystem>();
	Memory::ObjectPtr<GravitySystem> sGravity = world->system.Create<GravitySystem>();
	Memory::ObjectPtr<MoveSystem> sMove = world->system.Create<MoveSystem>();
	Memory::ObjectPtr<CollisionSystem> sCollision = world->system.Create<CollisionSystem>();
	Memory::ObjectPtr<CameraSystem> sCamera = world->system.Create<CameraSystem>();

	// Set the system's dependency.
	sRender->DependOn(sCamera);
	sRender->DependOn(sMove);

	sMove->DependOn(sCollision);
	sCollision->DependOn(sGravity);

	// System Graph
	// 
	// Gravity   Camera
	//    |        |
	// Collision   |
	//    |        |
	//   Move     /
	//     \     /
	//      Render

	// Build the system graph.
	const ECS::SystemRegistry::GraphType graph = world->system.BuildGraph();
	for (const auto& systemID : graph.GetSorted())
	{
		const auto system = world->system.Get(systemID);
		if (system)
		{
			LOGINFO() << "[TEST] System - " << system->GetTypeInfo()->GetTypeName();
		}
	}

	// Failed to build the system graph, cause the move system relyed on the render system.
	// The dependency makes circular graph.
	sMove->DependOn(sRender);
	world->system.BuildGraph();
	sMove->RemoveOn(sRender);

	// Register node and system on the scene.
	auto& scene = world->scene;

	scene.RegisterNode(nRender);
	scene.RegisterNode(nPhysics);

	scene.RegisterSystem(sRender);
	scene.RegisterSystem(sGravity);
	scene.RegisterSystem(sMove);

	ECS::TimeStep timeStep;
	world->Update(timeStep);

	// Failed to register the collision system, cause the collision node isn't registed.
	// So, if the collision system is registed, it will just update the other scene's node data.
	scene.RegisterSystem(sCollision);

	scene.RegisterNode(nCollision);
	scene.RegisterSystem(sCollision);

	world->Update(timeStep);

	Memory::Collect();

	LOGINFO() << "[TEST] Done";
}

void Print()
{
	LOGINFO() << "[ Print Type Info ]";

	for (const auto& [hash, typeInfo] : Reflection::TypeManager::GetHandle().GetTypeMap())
	{
		LOGINFO() << "[TEST] Type : " << typeInfo->GetTypeName();

		for (const auto& [name, property] : typeInfo->GetProperties())
		{
			LOGINFO() << "[TEST] Property : " << name;
		}

		for (const auto& [name, methd] : typeInfo->GetMethods())
		{
			LOGINFO() << "[TEST] Method : " << name;
		}
	}

	LOGINFO() << " ";
}

int MAIN()
{
	Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Type);

	Memory::Init(1024, 10000);

	// Print(); Reflection Test
	
	Test();

	Memory::Collect();
	Memory::Release();

	system("pause");

	return 0;
}