#include "ComponentManager.h"

ComponentManager::ComponentManager() {}

ComponentManager::~ComponentManager() {}

void ComponentManager::Init()
{
    m_IComponentTypeData.clear();
    m_IComponentMPtrData.clear();
    m_IComponentIDData.clear();

    Log::Info(" Initialize Component Manager ");
}

void ComponentManager::Destroy()
{
    Log::Info(" Destroy Component Manager ");
}

ComponentManager::TypeUnSet& ComponentManager::GetTypeData(){ return m_IComponentTypeData; }

MyUUIDUnSet& ComponentManager::GetIDData( const std::type_info* Type )
{
    bool Check = HasIDSet( Type );
    if ( !Check ) CreateIDSet( Type );
    return m_IComponentIDData[ Type ];
}

bool ComponentManager::HasIDSet( const std::type_info* Type )
{
    auto ITR = m_IComponentTypeData.find( Type );
    if ( ITR != m_IComponentTypeData.end() ) return true;
    else return false;
}

void ComponentManager::CreateIDSet( const std::type_info* Type )
{
    m_IComponentTypeData.insert( Type );

    m_IComponentIDData[ Type ] = MyUUIDUnSet();
    m_IComponentIDData[ Type ].clear();
}

ComponentManager& ComponentManager::GetHandle() { return m_ComponentManager; }
ComponentManager ComponentManager::m_ComponentManager;