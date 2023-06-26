#include "NodeManager.h"

NodeManager::NodeManager() {}

NodeManager::~NodeManager() {}

void NodeManager::Init()
{
    Log::Info(" Initialize Node Manager ");
}

void NodeManager::Destroy()
{
    Log::Info(" Destroy Node Manager ");
}

NodeManager::TypepINodeMap& NodeManager::GetData() { return m_Data; }
NodeManager& NodeManager::GetHandle() { return m_NodeManager; }
NodeManager NodeManager::m_NodeManager;