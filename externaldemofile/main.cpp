#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>
#include <ECSProject/Manager.h>
#include <ECSProject/MyUUID.h>
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

    // Create Scene
    MemoryPtr<Scene> Main = SceneManager::GetHandle().Create();

    // Create Entity and Register in Main Scene
    MemoryPtr<Entity> Object = EntityManager::GetHandle().Create();

    // Create Component
    MemoryPtr<MeshComponent> Mesh = ComponentManager::GetHandle().Create<MeshComponent>();
    MemoryPtr<TransformComponent> Trans = ComponentManager::GetHandle().Create<TransformComponent>();

    // Add Mesh Component and Transfrom Component to Object Entity
    Object->AddComponent<MeshComponent>( Mesh->GetID() );
    Object->AddComponent<TransformComponent>( Trans->GetID() );

    // Create RenderNode through Object Entity
    MemoryPtr<RenderNode> Node_1 = NodeManager::GetHandle().Create<RenderNode>( Object );
    Main->RegisterNode<RenderNode>( Node_1->GetID() );

    // Register some system in Main Scene
    Main->RegisterSystem( Render->GetID() );
    Main->RegisterSystem( Physics->GetID() );

    // Frame in Scene
    Main->Update( 0.0f );

    // Create PhysicsNode through Object Entity
    MemoryPtr<PhysicsNode> Node_2 = NodeManager::GetHandle().Create<PhysicsNode>( Object );
    Main->RegisterNode<PhysicsNode>( Node_2->GetID() );

    // Frame in Scene
    Main->Update( 0.0f );


    SceneManager::GetHandle().Destroy();
    SystemManager::GetHandle().Destroy();
    EntityManager::GetHandle().Destroy();
    NodeManager::GetHandle().Destroy();
    ComponentManager::GetHandle().Destroy();

    MemoryManager::GetHandle().Destroy();
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