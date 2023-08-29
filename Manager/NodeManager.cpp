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

NodeManager::TypeUnSet& NodeManager::GetTypeData() { return m_INodeTypeData; }

MyUUIDUnSet& NodeManager::GetIDData( const std::type_info* Type )
{
    bool Check = HasIDSet( Type );
    if ( !Check ) CreateIDSet( Type );
    return m_INodeIDData[ Type ];
}

bool NodeManager::HasIDSet( const std::type_info* Type )
{
    auto ITR = m_INodeTypeData.find( Type );
    if ( ITR != m_INodeTypeData.end() ) return true;
    else return false;
}

void NodeManager::CreateIDSet( const std::type_info* Type )
{
    m_INodeTypeData.insert( Type );

    m_INodeIDData[ Type ] = MyUUIDUnSet();
    m_INodeIDData[ Type ].clear();
}

NodeManager& NodeManager::GetHandle() { return m_NodeManager; }
NodeManager NodeManager::m_NodeManager;