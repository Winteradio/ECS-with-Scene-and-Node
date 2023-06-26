#include "ComponentManager.h"

ComponentManager::ComponentManager() {}

ComponentManager::~ComponentManager() {}

void ComponentManager::Init()
{
    Log::Info(" Initialize Component Manager ");
}

void ComponentManager::Destroy()
{
    Log::Info(" Destroy Component Manager ");
}

ComponentManager::TypepIComponentMap& ComponentManager::GetData() { return m_Data; }
ComponentManager& ComponentManager::GetHandle() { return m_ComponentManager; }
ComponentManager ComponentManager::m_ComponentManager;