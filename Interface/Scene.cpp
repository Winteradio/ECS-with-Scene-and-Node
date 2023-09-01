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
    m_NodeTypeData.clear();
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

        if ( m_NodeIDData[ Type ].empty() ) 
        {
            auto TypeITR = m_NodeTypeData.find( Type );
            if ( TypeITR != m_NodeTypeData.end() ) m_NodeTypeData.erase( TypeITR );
        }
    }
}

bool Scene::HasNode( const std::type_info* Type, MyUUID& ID )
{
    bool Check = HasNodeType( Type );
    if ( !Check )
    {
        m_NodeTypeData.insert( Type );
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
    auto ITR = m_NodeTypeData.find( Type );
    if ( ITR != m_NodeTypeData.end() ) return true;
    else return false;
}

void Scene::RegisterSystem( MyUUID& ID )
{
    MemoryPtr<ISystem> System = SystemManager::GetHandle().GetSystem( ID );

    RegisterSystem( System->GetNodeType(), ID );
}

void Scene::RegisterSystem( const std::type_info* Type, MyUUID& ID )
{
    bool Check = HasSystem( ID );

    if ( !Check )
    {
        auto IDITR = m_PendingSystemIDData.find( Type );
        if ( IDITR == m_PendingSystemIDData.end() )
        {
            m_PendingSystemIDData[ Type ] = MyUUIDUnSet();
        }

        m_PendingSystemIDData[ Type ].insert( ID );
        CheckSystemValidity( Type );
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
    auto ITR = m_SystemIDData.find( ID );

    if ( ITR != m_SystemIDData.end() ) return true;
    else return false;
}

MyUUIDUnSet& Scene::GetNodeIDData( const std::type_info* Type ) 
{ 
    auto ITR = m_NodeTypeData.find( Type );
    if ( ITR == m_NodeTypeData.end() )
    {
        throw Except( " Scene | %s | There is no Node type of %s ", __FUNCTION__, Type->name() );
    }

    return m_NodeIDData[ Type ];
}

void Scene::CheckSystemValidity( const std::type_info* Type )
{
    bool Check = HasNodeType( Type );
    if ( !Check ) return ;

    auto ITR = m_PendingSystemIDData.find( Type );
    if ( ITR == m_PendingSystemIDData.end() ) return;

    for ( auto ID : m_PendingSystemIDData[ Type ] )
    {
        m_SystemIDData.insert( ID );
    }

    m_PendingSystemIDData.erase( ITR );

    UpdateSequence();
}

void Scene::UpdateSequence()
{
    m_SystemSequence = SystemManager::GetHandle().UpdateSequence( m_SystemIDData );
}

Scene::TypeUnSet& Scene::GetNodeTypeData() { return m_NodeTypeData; }
MyUUIDUnSet& Scene::GetSystemIDData() { return m_SystemIDData; }

std::string Scene::GetName() { return m_Name; }
int Scene::GetIndex() { return m_Index; }

void Scene::SetName( std::string Name ) { m_Name = Name; }
void Scene::SetIndex( int Index ) { m_Index = Index; }

