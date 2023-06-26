#include <ECSProject/Manager.h>
#include <ECSProject/MyUUID.h>
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

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    Log::Info(" Windows ");
#elif __linux__
int main()
{
    Log::Info(" Linux ");
#endif
    MemoryManager::GetHandle().Init();

    // Initialization each Manager
    SceneManager::GetHandle().Init();
    SystemManager::GetHandle().Init();
    EntityManager::GetHandle().Init();
    NodeManager::GetHandle().Init();
    ComponentManager::GetHandle().Init();

    // Create Scene
    Scene* Main = SceneManager::GetHandle().Create();

    // Create Entity and Register in Main Scene
    Entity* Num_1 = EntityManager::GetHandle().Create();
    Entity* Num_2 = EntityManager::GetHandle().Create();

    TransformComponent* Trans = ComponentManager::GetHandle().Create<TransformComponent>();

    // Create RenderNode using Num_1
    RenderNode* Node = NodeManager::GetHandle().Create<RenderNode>( *Num_1 );

    // Create each System and Set Dependency each other
    ISystem* Render = SystemManager::GetHandle().Create<RenderSystem>();
    ISystem* Move = SystemManager::GetHandle().Create<MoveSystem>();
    ISystem* Collision = SystemManager::GetHandle().Create<CollisionSystem>();
    ISystem* Physics = SystemManager::GetHandle().Create<PhysicsSystem>();
    ISystem* Camera = SystemManager::GetHandle().Create<CameraSystem>();

    SystemManager::GetHandle().SetDependency( Physics->GetID(), Collision->GetID() );
    SystemManager::GetHandle().SetDependency( Collision->GetID(), Move->GetID() );
    SystemManager::GetHandle().SetDependency( Move->GetID(), Render->GetID() );
    SystemManager::GetHandle().SetDependency( Camera->GetID(), Render->GetID() );

    // This Dependency relation with RenderySystem and PhysicsSystem occur circlic graph
    // So, this relation is deleted by system manager
    SystemManager::GetHandle().SetDependency( Render->GetID(), Physics->GetID() ); 

    // Register some system in Main Scene
    Main->RegisterSystem( Render->GetID() );
    Main->RegisterSystem( Physics->GetID() );
    Main->RegisterSystem( Camera->GetID() );

    // Frame in Scene
    Main->Update( 0.0f );

    Log::Print();
    
    return 0;
};