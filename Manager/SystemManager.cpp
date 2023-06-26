#include "SystemManager.h"

SystemManager::SystemManager() {}
SystemManager::~SystemManager() {}

void SystemManager::Init()
{
    Log::Info(" Initialize System Manager ");
}

void SystemManager::Destroy()
{
    Log::Info(" Destroy System Manager ");
}

ISystem* SystemManager::GetSystem( MyUUID ID )
{
    bool Check = HasSystem( ID );

    if ( !Check ) return nullptr;

    return m_Data[ ID ];
}

bool SystemManager::HasSystem( MyUUID ID )
{
    if ( m_Data.find( ID ) != m_Data.end() ) return true;
    else return false;
}

void SystemManager::Remove( MyUUID ID )
{
    bool Check = HasSystem( ID );
    if ( Check )
    {
        auto DataITR = m_Data.find( ID );
        m_Data.erase( DataITR );

        auto DepIDSetMapITR = m_DependencyIDSetMap.find( ID );
        m_DependencyIDSetMap.erase( DepIDSetMapITR );

        for ( auto& [ SystemID, DepIDSet ] : m_DependencyIDSetMap )
        {
            auto DepIDITR = DepIDSet.find( ID );

            if ( DepIDITR != DepIDSet.end() )
            {
                DepIDSet.erase( DepIDITR );
            }
        }
    }
}

void SystemManager::SetDependency( MyUUID MainID, MyUUID DependencyID )
{
    bool Check = HasSystem( MainID ) && HasSystem( DependencyID );

    if ( !Check ) return ;

    m_DependencyIDSetMap[ MainID ].insert( DependencyID );

    MyUUIDSet TestIDSet;
    for ( auto [ ID, System ] : m_Data )
    {
        TestIDSet.insert( ID );
    }

    Check = TopologySort( TestIDSet );

    if ( !Check )
    {
        DeleteDependency( MainID, DependencyID );
    }
}

void SystemManager::DeleteDependency( MyUUID MainID, MyUUID DependencyID )
{
    bool Check = HasSystem( MainID );

    if ( Check )
    {
        auto ITR = m_DependencyIDSetMap[ MainID ].find( DependencyID );

        if ( ITR == m_DependencyIDSetMap[ MainID ].end() )
        {
            Log::Warn( " %s System has not %s System ", typeid( *GetSystem( MainID ) ).name(), typeid( *GetSystem( DependencyID) ).name() );
            return;
        }
        else
        {
            Log::Info( " Erase Dependency %s for %s ", typeid( *GetSystem( MainID ) ).name(), typeid( *GetSystem( DependencyID) ).name() );
            m_DependencyIDSetMap[ MainID ].erase( ITR );
        }
    }
}

void SystemManager::UpdateSequence( MyUUIDSet& SceneSystems, SystemSequence& SceneSequence )
{
    TopologySort( SceneSystems, SceneSequence );
}

bool SystemManager::TopologySort( MyUUIDSet& SceneSystems, SystemSequence& SceneSequence )
{
    MyUUIDQueue MainQueue, TempQueue;

    DependencyIDSetMap DepIDSetMap = CalculateDependency( SceneSystems );
    DependencyIndegreeMap DepIndegreeMap = CalculateIndegree( DepIDSetMap );

    for ( auto& [ SystemID, Indegree ] : DepIndegreeMap )
    {
        if ( Indegree == 0 )
        {
            MainQueue.push( SystemID );
            TempQueue.push( SystemID );
            Indegree--;
        }
    }

    while( !TempQueue.empty() )
    {
        MyUUID& TempID = TempQueue.front();
        for ( auto DependencyID : DepIDSetMap[ TempID ] )
        {
            DepIndegreeMap[ DependencyID ]--;

            if ( DepIndegreeMap[ DependencyID ] == 0 )
            {
                MainQueue.push( DependencyID );
                TempQueue.push( DependencyID );
                DepIndegreeMap[ DependencyID ]--;      
            }
        }

        TempQueue.pop();
    }

    if ( MainQueue.size() == SceneSystems.size() )
    {
        SceneSequence = SystemSequence();
        while( !MainQueue.empty() )
        {
            SceneSequence.push( GetSystem( MainQueue.front() ) );
            MainQueue.pop();
        }
        return true;
    }
    else
    {
        Log::Warn( " This Graph is circlic " );
        return false;
    }
}

SystemManager::DependencyIndegreeMap SystemManager::CalculateIndegree( DependencyIDSetMap& DepIDSetMap )
{
    DependencyIndegreeMap DepIndegreeMap;

    for ( auto [ SystemID, DependencyIDSet ] : DepIDSetMap )
    {
        DepIndegreeMap[ SystemID ] = 0;
    }

    for ( auto [ SystemID, DependencyIDSet ] : DepIDSetMap )
    {
        for ( auto DependencyID : DependencyIDSet )
        {
            DepIndegreeMap[ DependencyID ]++;
        }
    }

    return DepIndegreeMap;
}

SystemManager::DependencyIDSetMap SystemManager::CalculateDependency( MyUUIDSet& SceneSystems )
{
    DependencyIDSetMap DepIDSetMap;

    for ( MyUUID SystemID : SceneSystems )
    {
        bool Check = HasSystem( SystemID );

        if ( !Check )
        {
            auto ITR = SceneSystems.find( SystemID );
            SceneSystems.erase( SystemID );

            Log::Warn( " This System is not existed on System Manager " );
            continue;
        }

        DepIDSetMap[ SystemID ] = MyUUIDSet();
    }

    for ( MyUUID SystemID : SceneSystems )
    {
        for ( MyUUID DependencyID : m_DependencyIDSetMap[ SystemID ] )
        {
            RecursiveCheckDependency( SystemID, DependencyID, SceneSystems, DepIDSetMap );
        }
    }

    return DepIDSetMap;
}

void SystemManager::RecursiveCheckDependency( MyUUID MainID, MyUUID SystemID, MyUUIDSet& SceneSystems, DependencyIDSetMap& DepIDSetMap )
{
    if ( SceneSystems.find( SystemID ) != SceneSystems.end() )
    {
        DepIDSetMap[ MainID ].insert( SystemID );
        return;
    }

    for ( MyUUID DependencyID : m_DependencyIDSetMap[ SystemID ] )
    {
        RecursiveCheckDependency( MainID, DependencyID, SceneSystems, DepIDSetMap );
    }
}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager SystemManager::m_SystemManager;