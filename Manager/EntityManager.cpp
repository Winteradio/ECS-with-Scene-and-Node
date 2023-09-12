#include "EntityManager.h"

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {}

void EntityManager::Init()
{
    m_EntityMPtrData.clear();
    m_EntityIDData.clear();

    Log::Info(" Initialize Entity Manager ");    
}

void EntityManager::Destroy()
{
    Log::Info(" Destroy Entity Manager ");
}

MemoryPtr<Entity> EntityManager::Create()
{
    MyUUID ID;
    return Create( ID, "Default" );
}

MemoryPtr<Entity> EntityManager::Create( std::string Name )
{
    MyUUID ID;
    return Create( ID, Name );
}

MemoryPtr<Entity> EntityManager::Create( MyUUID ID )
{
    return Create( ID, "Default" );
}

MemoryPtr<Entity> EntityManager::Create( MyUUID ID, std::string Name )
{
    bool Check = HasEntity( ID );
    if ( Check )
    {
        throw Except( " EntityManager | %s | The Entity ID %s is already existed ", __FUNCTION__, ID.GetString().c_str() );
    }
    m_EntityIDData.insert( ID );
    m_EntityMPtrData[ ID ] = MemoryManager::GetHandle().Create<Entity>( ID, Name );
    
    Log::Info(" Entity Info ID : %s / Name : %s ", ID.GetString().c_str() , Name.c_str() );

    return GetEntity( ID );
}

void EntityManager::Remove( MyUUID& ID )
{
    bool Check = HasEntity( ID );
    if ( Check )
    {
        MemoryManager::GetHandle().Delete<Entity>( GetEntity( ID ) );

        auto IDITR = m_EntityIDData.find( ID );
        if ( IDITR != m_EntityIDData.end() ) m_EntityIDData.erase( IDITR );

        auto MPtrITR = m_EntityMPtrData.find( ID );
        if ( MPtrITR != m_EntityMPtrData.end() ) m_EntityMPtrData.erase( MPtrITR );
    }

}

bool EntityManager::HasEntity( MyUUID& ID )
{
    auto ITR = m_EntityIDData.find( ID );
    if ( ITR != m_EntityIDData.end() ) return true;
    else return false;
}

MemoryPtr<Entity> EntityManager::GetEntity( MyUUID& ID )
{
    bool Check = HasEntity( ID );

    if ( !Check )
    {
        throw Except( " EntityManager | %s | There is no Entity for %s ", __FUNCTION__, ID.GetString().c_str() );
    }

    return m_EntityMPtrData[ ID ];
}

MyUUIDUnSet& EntityManager::GetIDData() { return m_EntityIDData; }
EntityManager& EntityManager::GetHandle() { return m_EntityManager; }

EntityManager EntityManager::m_EntityManager;