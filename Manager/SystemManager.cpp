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

    return m_ISystemUnMap[ ID ];
}

bool SystemManager::HasSystem( MyUUID ID )
{
    auto ITR = m_ISystemUnMap.find( ID );
    if ( ITR != m_ISystemUnMap.end() ) return true;
    else return false;
}

void SystemManager::Remove( MyUUID ID )
{
    bool Check = HasSystem( ID );
    if ( Check )
    {
        {
            auto ITR = m_ISystemUnMap.find( ID );
            m_ISystemUnMap.erase( ITR );
        }

        {
            auto ITR = m_DependencyID.find( ID );
            m_DependencyID.erase( ITR );
        }

        for ( auto& [ SystemID, DepIDSet ] : m_DependencyID )
        {
            auto ITR = DepIDSet.find( ID );

            if ( ITR != DepIDSet.end() )
            {
                DepIDSet.erase( ITR );
            }
        }
    }
}

void SystemManager::SetDependency( MyUUID MainID, MyUUID DependencyID )
{
    bool Check = HasSystem( MainID ) && HasSystem( DependencyID );

    if ( !Check ) return;

    m_DependencyID[ MainID ].insert( DependencyID );

    MyUUIDUnSet TestIDSet;

    for ( auto [ ID, System ] : m_ISystemUnMap )
    {
        TestIDSet.insert( ID );
    }

    Check = TopologySort( TestIDSet );

    if ( !Check ) { DeleteDependency( MainID, DependencyID ); }
}

void SystemManager::DeleteDependency( MyUUID MainID, MyUUID DependencyID )
{
    bool Check = HasSystem( MainID );

    if ( Check )
    {
        auto ITR = m_DependencyID[ MainID ].find( DependencyID );

        if ( ITR == m_DependencyID[ MainID ].end() )
        {
            Log::Warn( " %s System has not %s System ", typeid( *GetSystem( MainID ) ).name(), typeid( *GetSystem( DependencyID) ).name() );
            return;
        }
        else
        {
            Log::Info( " Erase Dependency %s for %s ", typeid( *GetSystem( MainID ) ).name(), typeid( *GetSystem( DependencyID) ).name() );
            m_DependencyID[ MainID ].erase( ITR );
        }
    }
}

void SystemManager::UpdateSequence( MyUUIDUnSet& SceneSystems, ISystemQueue& SystemSequence )
{
    TopologySort( SceneSystems, SystemSequence );
}

bool SystemManager::TopologySort( MyUUIDUnSet& SceneSystems, ISystemQueue& SystemSequence )
{
    MyUUIDQueue MainQueue, TempQueue;

    DependencyIDUnSetUnMap DepIDMap = CalculateDependency( SceneSystems );
    DependencyIndegreeUnMap DepIndegreeMap = CalculateIndegree( DepIDMap );

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
        for ( auto DependencyID : DepIDMap[ TempID ] )
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
        SystemSequence = ISystemQueue();
        while( !MainQueue.empty() )
        {
            SystemSequence.push( GetSystem( MainQueue.front() ) );
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

SystemManager::DependencyIndegreeUnMap SystemManager::CalculateIndegree( DependencyIDUnSetUnMap& DepIDMap )
{
    DependencyIndegreeUnMap DepIndegreeMap;

    for ( auto [ SystemID, DependencyIDSet ] : DepIDMap )
    {
        DepIndegreeMap[ SystemID ] = 0;
    }

    for ( auto [ SystemID, DependencyIDSet ] : DepIDMap )
    {
        for ( auto DependencyID : DependencyIDSet )
        {
            DepIndegreeMap[ DependencyID ]++;
        }
    }

    return DepIndegreeMap;
}

SystemManager::DependencyIDUnSetUnMap SystemManager::CalculateDependency( MyUUIDUnSet& SceneSystems )
{
    DependencyIDUnSetUnMap DepIDMap;

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

        DepIDMap[ SystemID ] = MyUUIDUnSet();
    }

    for ( MyUUID SystemID : SceneSystems )
    {
        for ( MyUUID DependencyID : m_DependencyID[ SystemID ] )
        {
            RecursiveCheckDependency( SystemID, DependencyID, SceneSystems, DepIDMap );
        }
    }

    return DepIDMap;
}

void SystemManager::RecursiveCheckDependency( MyUUID MainID, MyUUID SystemID, MyUUIDUnSet& SceneSystems, DependencyIDUnSetUnMap& DepIDMap )
{
    if ( SceneSystems.find( SystemID ) != SceneSystems.end() )
    {
        DepIDMap[ MainID ].insert( SystemID );
        return;
    }

    for ( MyUUID DependencyID : m_DependencyID[ SystemID ] )
    {
        RecursiveCheckDependency( MainID, DependencyID, SceneSystems, DepIDMap );
    }
}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager SystemManager::m_SystemManager;