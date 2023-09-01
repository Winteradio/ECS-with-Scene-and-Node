#include "Manager.h"
#include "MyUUID.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>
#include <iostream>
#include <map>
#include <string>

#include "SystemList.h"
#include "ComponentList.h"
#include "NodeList.h"

#ifdef _WIN32
#include <windows.h>

void Example()
{
    MemoryManager::GetHandle().Init();

    // Initialization each Manager
    SceneManager::GetHandle().Init();
    SystemManager::GetHandle().Init();
    EntityManager::GetHandle().Init();
    NodeManager::GetHandle().Init();
    ComponentManager::GetHandle().Init();

    // Create Scene
    MemoryPtr<Scene> Main = SceneManager::GetHandle().Create();

    // Create Entity and Register in Main Scene
    MemoryPtr<Entity> Num_1 = EntityManager::GetHandle().Create();
    MemoryPtr<Entity> Num_2 = EntityManager::GetHandle().Create();

    MemoryPtr<Entity>& Num_Ref = EntityManager::GetHandle().GetEntity( Num_1->GetID() );

    // Create Component
    MemoryPtr<MeshComponent> Mesh = ComponentManager::GetHandle().Create<MeshComponent>( MyUUID() );
    MemoryPtr<TransformComponent> Trans = ComponentManager::GetHandle().Create<TransformComponent>( MyUUID() );

    // Add Mesh Component and Transfrom Component to Num_1 Entity
    Num_1->AddComponent<MeshComponent>( Mesh->GetID() );
    Num_1->AddComponent<TransformComponent>( Trans->GetID() );

    // Create RenderNode through Num_1 Entity,
    // So, Node_1 uses Mesh Component and Transfor Component in Num_1 Entity
    MemoryPtr<RenderNode> Node_1 = NodeManager::GetHandle().Create<RenderNode>( Num_1 );
    MemoryPtr<CameraNode> Node_2 = NodeManager::GetHandle().Create<CameraNode>( Num_1 );
    MemoryPtr<CollisionNode> Node_3 = NodeManager::GetHandle().Create<CollisionNode>( Num_1 );
    MemoryPtr<PhysicsNode> Node_4 = NodeManager::GetHandle().Create<PhysicsNode>( Num_1 );

    MemoryPtr<RenderNode> Node_Ref = NodeManager::GetHandle().GetNode<RenderNode>( Node_1->GetID() );

    // Register Node_1 Render Node to Main Scene
    Main->RegisterNode<RenderNode>( Node_1->GetID() );
    Main->RegisterNode<CameraNode>( Node_2->GetID() );
    Main->RegisterNode<CollisionNode>( Node_3->GetID() );
    Main->RegisterNode<PhysicsNode>( Node_4->GetID() );

    // Create each System using System Manager
    MemoryPtr<ISystem> Render = SystemManager::GetHandle().Create<RenderSystem>();
    MemoryPtr<ISystem> Move = SystemManager::GetHandle().Create<MoveSystem>();
    MemoryPtr<ISystem> Collision = SystemManager::GetHandle().Create<CollisionSystem>();
    MemoryPtr<ISystem> Physics = SystemManager::GetHandle().Create<PhysicsSystem>();
    MemoryPtr<ISystem> Camera = SystemManager::GetHandle().Create<CameraSystem>();

    // Set dependency eacy system
    SystemManager::GetHandle().SetDependency( Physics->GetID(), Collision->GetID() );
    SystemManager::GetHandle().SetDependency( Collision->GetID(), Move->GetID() );
    SystemManager::GetHandle().SetDependency( Move->GetID(), Render->GetID() );
    SystemManager::GetHandle().SetDependency( Camera->GetID(), Render->GetID() );

    // This Dependency relation with RenderySystem and PhysicsSystem occur circlic graph
    // So, this relation is deleted by system manager
    SystemManager::GetHandle().SetDependency( Render->GetID(), Physics->GetID() ); 

    // Register some system in Main Scene
    Main->RegisterSystem( Render->GetID() );
    Main->RegisterSystem( Move->GetID() );
    Main->RegisterSystem( Physics->GetID() );
    Main->RegisterSystem( Camera->GetID() );
    Main->RegisterSystem( Collision->GetID() );

    // Frame in Scene
    Main->Update( 0.0f );
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    Log::Info(" Windows ");
#elif __linux__
int main()
{
    Log::Info(" Linux ");
#endif
    try
    {
        Example();
    }
    catch ( const Except& e )
    {
        Log::Error( e.what() );
    }

    Log::Print();
    
    return 0;
};