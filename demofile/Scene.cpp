#include "Scene.h"

#include "SystemManager.h"

Scene::Scene() {}
Scene::Scene( std::string Name ) : m_Name( Name ) {}
Scene::Scene( int Index ) : m_Index( Index ) {}
Scene::Scene( int Index, std::string Name ) : m_Index( Index ), m_Name( Name ) {}
Scene::~Scene() {}

void Scene::Init() {}

void Scene::Destroy()
{
    m_NodeIDData.clear();
    m_SystemIDData.clear();
}

void Scene::Update( float DeltaTime )
{
    for ( auto& SystemID : m_SystemSequence )
    {
        MemoryPtr<ISystem>& System = SystemManager::GetHandle().GetSystem( SystemID );
        System->Update( DeltaTime, m_Index, GetNodeIDData( System->GetNodeType() ) );
    }
}

void Scene::RegisterNode( const std::type_info* Type, MyUUID& ID )
{
    bool Check = HasNode( Type, ID );

    if ( !Check ) m_NodeIDData[ Type ].insert( ID );
}

void Scene::RemoveNode( const std::type_info* Type, MyUUID& ID )
{
    bool Check = HasNode( Type, ID );

    if ( Check )
    {
        auto IDITR = m_NodeIDData[ Type ].find( ID );
        if ( IDITR != m_NodeIDData[ Type ].end() ) m_NodeIDData[ Type ].erase( IDITR );

        if ( m_NodeIDData[ Type ].empty() ) m_NodeIDData.erase( Type );
    }
}

bool Scene::HasNode( const std::type_info* Type, MyUUID& ID )
{
    bool Check = HasNodeType( Type );
    if ( !Check )
    {
        m_NodeIDData[ Type ] = MyUUIDUnSet();

        CheckSystemValidity( Type );
        return false; 
    }

    auto IDITR = m_NodeIDData[ Type ].find( ID );
    if ( IDITR != m_NodeIDData[ Type ].end() ) return true;
    else return false;
}

bool Scene::HasNodeType( const std::type_info* Type )
{
    auto ITR = m_NodeIDData.find( Type );
    if ( ITR != m_NodeIDData.end() ) return true;
    else return false;
}

void Scene::RegisterSystem( MyUUID& ID )
{
    MemoryPtr<ISystem> System = SystemManager::GetHandle().GetSystem( ID );

    RegisterSystem( System->GetNodeType(), ID );
}

void Scene::RegisterSystem( const std::type_info* NodeType, MyUUID& ID )
{
    bool Check = HasSystem( ID );

    if ( !Check )
    {
        auto TypeITR == m_SystemIDData[ false ].find( NodeType );
        if ( TypeITR == m_SystemIDData[ false ].end() )
        {
            m_SystemIDData[ false ][ NodeType ] = MyUUIDUnSet();
        }

        m_SystemIDData[ false ][ NodeType ].insert( ID );
        CheckSystemValidity( NodeType );
    }
}

void Scene::RemoveSystem( MyUUID& ID )
{
    bool Check = HasSystem( ID );

    if ( Check )
    {
        auto MainIDITR = m_SystemIDData.find( ID );
        m_SystemIDData.erase( MainIDITR );

        UpdateSequence();
    }
}

bool Scene::HasSystem( MyUUID& ID )
{
    auto IDITR = m_SystemIDData.find( ID );

    if ( IDITR != m_SystemIDData.end() ) return true;
    else return false;
}

void Scene::CheckSystemValidity( const std::type_info* NodeType )
{
    bool Check = HasNodeType( NodeType );
    if ( !Check ) return;

    auto TypeITR = m_PendingSystemIDData.find( NodeType );
    if ( TypeITR == m_PendingSystemIDData.end() ) return;

    for ( auto ID : m_PendingSystemIDData[ NodeType ] )
    {
        m_SystemIDData.insert( ID );
    }

    m_PendingSystemIDData.erase( TypeITR );

    UpdateSequence();
}

void Scene::UpdateSequence()
{
    m_SystemSequence = SystemManager::GetHandle().UpdateSequence( m_SystemIDData );
}

Scene::TypeUnSet Scene::GetNodeTypeData() 
{
    TypeUnSet NodeTypeData;

    for ( auto [ Type, IDSet] : m_NodeIDData )
    {
        NodeTypeData.insert( Type );
    }

    return NodeTypeData; 
}

MyUUIDUnSet& Scene::GetNodeIDData( const std::type_info* Type ) 
{ 
    auto TypeITR = m_NodeIDData.find( Type );
    if ( TypeITR == m_NodeIDData.end() )
    {
        throw Except( " Scene | %s | There is no Node type of %s ", __FUNCTION__, Type->name() );
    }

    return m_NodeIDData[ Type ];
}

MyUUIDUnSet& Scene::GetSystemIDData() { return m_SystemIDData; }

std::string Scene::GetName() { return m_Name; }
int Scene::GetIndex() { return m_Index; }

void Scene::SetName( std::string Name ) { m_Name = Name; }
void Scene::SetIndex( int Index ) { m_Index = Index; }

