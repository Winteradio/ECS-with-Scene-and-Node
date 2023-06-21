#include "SystemManager.h"

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Init()
{

}

void SystemManager::Destroy()
{
    
}

void SystemManager::SetDependency( ISystem*& Main, ISystem*& Dependency )
{

}

void SystemManager::DeleteDependency( ISystem*& Main, ISystem*& Dependency )
{

}

bool SystemManager::Sort( MyUUID SceneID )
{
    return true;
}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager SystemManager::m_SystemManager;