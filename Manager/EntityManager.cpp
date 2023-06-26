#include "EntityManager.h"

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {}

void EntityManager::Init()
{
    Log::Info(" Init - Entity Manager ");
}

void EntityManager::Destroy()
{
    m_Data.clear();
}

Entity* EntityManager::Create()
{
    MyUUID ID;
    ID.Init();
    return Create( ID, "Default" );
}

Entity* EntityManager::Create( std::string Name )
{
    MyUUID ID;
    ID.Init();
    return Create( ID, Name );
}

Entity* EntityManager::Create( MyUUID ID )
{
    return Create( ID, "Default" );
}

Entity* EntityManager::Create( MyUUID ID, std::string Name )
{
    Entity* Object = MemoryManager::GetHandle().Allocate<Entity>( ID, Name );
    Log::Info(" Entity Info ID : %s / Name : %s ", ID.GetString().c_str() , Name.c_str() );

    m_Data[ ID ] = Object;

    return GetEntity( ID );
}

void EntityManager::Remove( MyUUID ID )
{
    bool Check = HasEntity( ID );
    if ( Check )
    {
        auto EntityITR = m_Data.find( ID );
        m_Data.erase( EntityITR );
    }

}

bool EntityManager::HasEntity( MyUUID ID )
{
    if ( m_Data.find( ID ) != m_Data.end() ) return true;
    else return false;
}

Entity* EntityManager::GetEntity( MyUUID ID )
{
    bool Check = HasEntity( ID );
    if ( Check )
    {
        return m_Data[ ID ];
    }
    else
    {
        Log::Warn(" There is no Entity for %s ", ID.GetString().c_str() );
        return nullptr;
    }
}

EntityManager::pEntityMap& EntityManager::GetData() { return m_Data; }

EntityManager& EntityManager::GetHandle() { return m_EntityManager; }

EntityManager EntityManager::m_EntityManager;