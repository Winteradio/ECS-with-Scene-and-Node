#include "EntityManager.h"

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {}

void EntityManager::Init()
{
    Log::Info(" Initialize Entity Manager ");
}

void EntityManager::Destroy()
{
    m_EntityMPtrUnMap.clear();
    m_EntityIDUnSet.clear();

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
    m_EntityIDUnSet.insert( ID );
    m_EntityMPtrUnMap[ ID ] = MemoryManager::GetHandle().Create<Entity>( ID, Name );
    
    Log::Info(" Entity Info ID : %s / Name : %s ", ID.GetString().c_str() , Name.c_str() );

    return GetEntity( ID );
}

void EntityManager::Remove( MyUUID ID )
{
    bool Check = HasEntity( ID );
    if ( Check )
    {
        MemoryManager::GetHandle().Delete<Entity>( GetEntity( ID ) );

        {
            auto ITR = m_EntityMPtrUnMap.find( ID );
            m_EntityMPtrUnMap.erase( ITR );
        }

        {
            auto ITR = m_EntityIDUnSet.find( ID );
            m_EntityIDUnSet.erase( ITR );
        }
    }

}

bool EntityManager::HasEntity( MyUUID ID )
{
    auto ITR = m_EntityIDUnSet.find( ID );
    if ( ITR != m_EntityIDUnSet.end() ) return true;
    else return false;
}

MemoryPtr<Entity> EntityManager::GetEntity( MyUUID ID )
{
    bool Check = HasEntity( ID );

    if ( !Check )
    {
        throw Except( " EntityManager | %s | There is no Entity for %s ", __FUNCTION__, ID.GetString().c_str() );
    }

    return m_EntityMPtrUnMap[ ID ];
}

MyUUIDUnSet& EntityManager::GetIDData() { return m_EntityIDUnSet; }
EntityManager& EntityManager::GetHandle() { return m_EntityManager; }

EntityManager EntityManager::m_EntityManager;