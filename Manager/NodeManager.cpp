#include "NodeManager.h"

NodeManager::NodeManager() {}

NodeManager::~NodeManager() {}

void NodeManager::Init()
{
    m_INodeMPtrData.clear();
    m_INodeIDData.clear();
    m_INodeTypeData.clear();

    Log::Info(" Initialize Node Manager ");
}

void NodeManager::Destroy()
{
    Log::Info(" Destroy Node Manager ");
}

NodeManager::TypeUnSet& NodeManager::GetTypeData() { return m_INodeTypeData; }

MyUUIDUnSet& NodeManager::GetIDData( const std::type_info* Type )
{
    bool Check = HasMPtrMap( Type );
    if ( !Check ) CreateMPtrMap( Type );
    return m_INodeIDData[ Type ];
}

NodeManager::MyUUIDINodeMPtrUnMap& NodeManager::GetMPtrMapData( const std::type_info* Type )
{
    bool Check = HasMPtrMap( Type );
    if ( !Check ) CreateMPtrMap( Type );
    return m_INodeMPtrData[ Type ];
}

bool NodeManager::HasMPtrMap( const std::type_info* Type )
{
    auto ITR = m_INodeTypeData.find( Type );
    if ( ITR != m_INodeTypeData.end() ) return true;
    else return false;
}

void NodeManager::CreateMPtrMap( const std::type_info* Type )
{
    m_INodeTypeData.insert( Type );

    m_INodeIDData[ Type ] = MyUUIDUnSet();
    m_INodeIDData[ Type ].clear();

    m_INodeMPtrData[ Type ] = MyUUIDINodeMPtrUnMap();
    m_INodeMPtrData[ Type ].clear();
}

NodeManager& NodeManager::GetHandle() { return m_NodeManager; }
NodeManager NodeManager::m_NodeManager;