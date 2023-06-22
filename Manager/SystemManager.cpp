#include "SystemManager.h"

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Init()
{
    Log::Info(" Initialize System Manager ");
}

void SystemManager::Destroy()
{
    Log::Info(" Destroy System Manager ");
}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager SystemManager::m_SystemManager;