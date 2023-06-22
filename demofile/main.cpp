#include <ECSProject/Manager.h>
#include <LogProject/Log.h>
#include <iostream>

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

    SceneManager::GetHandle().Init();
    SystemManager::GetHandle().Init();
    EntityManager::GetHandle().Init();
    NodeManager::GetHandle().Init();
    ComponentManager::GetHandle().Init();

    Log::Print();
    
    return 0;
};