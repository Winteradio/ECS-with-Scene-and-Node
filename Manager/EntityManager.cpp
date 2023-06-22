#include "EntityManager.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

void EntityManager::Init()
{
    Log::Info(" Initialize Entity Manager ");
}

void EntityManager::Destroy()
{
    Log::Info(" Destroy Entity Manager ");
}

EntityManager& EntityManager::GetHandle() { return m_EntityManager; }
EntityManager EntityManager::m_EntityManager;