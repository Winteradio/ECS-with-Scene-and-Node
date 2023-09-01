#include "SystemManager.h"

SystemManager::SystemManager() {}
SystemManager::~SystemManager() {}

void SystemManager::Init()
{
    m_ISystemMPtrData.clear();
    m_DependencyIDData.clear();
    m_SystemIndexData.clear();

    Log::Info(" Initialize System Manager ");
}

void SystemManager::Destroy()
{
    Log::Info(" Destroy System Manager ");
}

MemoryPtr<ISystem> SystemManager::GetSystem( MyUUID& ID )
{
    bool Check = HasSystem( ID );
    if ( !Check ) { throw Except( " SystemManager | %s | This System is not  existed ", ID.GetString().c_str() ); }

    return m_ISystemMPtrData[ ID ];
}

bool SystemManager::HasSystem( MyUUID& ID )
{
    auto ITR = m_ISystemMPtrData.find( ID );
    if ( ITR != m_ISystemMPtrData.end() ) return true;
    else return false;
}

void SystemManager::Remove( MyUUID& ID )
{
    bool Check = HasSystem( ID );
    if ( Check )
    {
        auto IndexITR = m_SystemIndexData.find( ID );
        if ( IndexITR != m_SystemIndexData.end() ) m_SystemIndexData.erase( IndexITR );

        auto MPtrITR = m_ISystemMPtrData.find( ID );
        if ( MPtrITR != m_ISystemMPtrData.end() ) m_ISystemMPtrData.erase( MPtrITR );

        auto DepITR = m_DependencyIDData.find( ID );
        if ( DepITR != m_DependencyIDData.end() ) m_DependencyIDData.erase( DepITR );

        for ( auto& [ DependencyID, DependentIDData ] : m_DependencyIDData )
        {
            auto ITR = DependentIDData.find( ID );
            if ( ITR != DependentIDData.end() ) DependentIDData.erase( ITR );
        }
    }
}

void SystemManager::SetDependency( MyUUID& MainID, MyUUID& DependentID )
{
    bool Check = HasSystem( MainID ) && HasSystem( DependentID );
    if ( !Check ) return;

    m_DependencyIDData[ MainID ].insert( DependentID );

    MyUUIDUnSet TestIDData;
    for ( auto [ ID, System ] : m_ISystemMPtrData ) 
    { 
        TestIDData.insert( ID ); 
    }

    Check = TopologySort( TestIDData );
    if ( !Check ) { DeleteDependency( MainID, DependentID ); }
}

void SystemManager::DeleteDependency( MyUUID& MainID, MyUUID& DependentID )
{
    bool Check = HasSystem( MainID ) && HasSystem( DependentID );
    if ( Check )
    {
        auto ITR = m_DependencyIDData[ MainID ].find( DependentID );
        if ( ITR != m_DependencyIDData[ MainID ].end() )
        {
            Log::Info( " Erase Dependency %s for %s ", typeid( *GetSystem( MainID ) ).name(), typeid( *GetSystem( DependentID) ).name() );
            m_DependencyIDData[ MainID ].erase( ITR );
        }
    }
}

MyUUIDVector SystemManager::UpdateSequence( MyUUIDUnSet& SystemIDData )
{
    MyUUIDVector SystemSequence;

    for ( auto& ID : SystemIDData ) 
    { 
        SystemSequence.push_back( ID ); 
    }

    std::sort( SystemSequence.begin(), SystemSequence.end(), 
        [this]( const MyUUID& ID1, const MyUUID& ID2 )
        { 
            return m_SystemIndexData[ ID1 ] < m_SystemIndexData[ ID2 ]; 
        } 
    );

    return SystemSequence;
}

bool SystemManager::TopologySort( MyUUIDUnSet& SystemIDData )
{
    MyUUIDQueue TopoQueue;
    DependentIndegreeUnMap TopoIndegreeData = CalculateIndegree( m_DependencyIDData );

    int Index = 0;
    for ( auto& [ ID, Indegree ] : TopoIndegreeData )
    {
        if ( Indegree == 0 )
        {
            m_SystemIndexData[ ID ] = ++Index;

            TopoQueue.push( ID );
            Indegree--;
        }
    }

    while( !TopoQueue.empty() )
    {
        MyUUID& ID = TopoQueue.front();
        for ( auto& DependencyID : m_DependencyIDData[ ID ] )
        {
            TopoIndegreeData[ DependencyID ]--;

            if ( TopoIndegreeData[ DependencyID ] == 0 )
            {
                m_SystemIndexData[ DependencyID ] = ++Index;
                TopoQueue.push( DependencyID );
                TopoIndegreeData[ DependencyID ]--;      
            }
        }

        TopoQueue.pop();
    }

    if ( Index == SystemIDData.size() ) return true;
    else
    {
        Log::Warn( " This Graph is circlic " );
        return false;
    }
}

SystemManager::DependentIndegreeUnMap SystemManager::CalculateIndegree( DependencyIDUnSetUnMap& DependencyIDData )
{
    DependentIndegreeUnMap DependentIndegreeData;

    for ( auto& [ ID, IDData ] : DependencyIDData ) { DependentIndegreeData[ ID ] = 0; }

    for ( auto& [ ID, IDData ] : DependencyIDData )
    {
        for ( auto& DependencyID : IDData )
        {
            DependentIndegreeData[ DependencyID ]++;
        }
    }

    return DependentIndegreeData;
}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager SystemManager::m_SystemManager;