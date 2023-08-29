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

    ClearSequence( m_MainSequence );
    ClearSequence( m_DoneSequence );
}

void Scene::Update( float DeltaTime )
{
    bool Check;
    while( !m_MainSequence.empty() )
    {
        ISystem* System = m_MainSequence.front();

        System->Update( DeltaTime, m_Index, GetNodeIDData( System->GetNodeType() ) );

        m_MainSequence.pop();
        m_DoneSequence.push( System );
    }

    while( !m_DoneSequence.empty() )
    {
        m_MainSequence.push( m_DoneSequence.front() );
        m_DoneSequence.pop();
    }
}

void Scene::RegisterNode( const std::type_info* Type, MyUUID ID )
{
    bool Check = HasNode( Type, ID );

    if ( !Check ) m_NodeIDData[ Type ].insert( ID );
}

void Scene::RemoveNode( const std::type_info* Type, MyUUID ID )
{
    bool Check = HasNode( Type, ID );

    if ( Check )
    {
        {
            auto ITR = m_NodeIDData[ Type ].find( ID );
            m_NodeIDData[ Type ].erase( ITR );
        }

        if ( m_NodeIDData[ Type ].empty() ) 
        {
            auto ITR = m_NodeTypeData.find( Type );
            m_NodeTypeData.erase( ITR );
        }
    }
}

bool Scene::HasNode( const std::type_info* Type, MyUUID ID )
{
    {
        auto ITR = m_NodeTypeData.find( Type );
        if ( ITR == m_NodeTypeData.end() )
        {
            m_NodeTypeData.insert( Type );
            m_NodeIDData[ Type ] = MyUUIDUnSet();

            CheckSystemValidity( Type );
            return false;
        }
    }

    {
        auto ITR = m_NodeIDData[ Type ].find( ID );
        if ( ITR != m_NodeIDData[ Type ].end() ) return true;
        else return false;
    }
}

bool Scene::HasNodeType( const std::type_info* Type )
{
    auto ITR = m_NodeTypeData.find( Type );
    if ( ITR != m_NodeTypeData.end() ) return true;
    else return false;
}

void Scene::RegisterSystem( MyUUID ID )
{
    ISystem* System = SystemManager::GetHandle().GetSystem( ID );

    RegisterSystem( System->GetNodeType(), ID );
}

void Scene::RegisterSystem( const std::type_info* Type, MyUUID ID )
{
    bool Check = HasSystem( ID );

    if ( !Check )
    {
        auto ITR = m_PendingSystemIDData.find( Type );
        if ( ITR == m_PendingSystemIDData.end() )
        {
            m_PendingSystemIDData[ Type ] = MyUUIDUnSet();
        }

        m_PendingSystemIDData[ Type ].insert( ID );
        CheckSystemValidity( Type );
    }
}

void Scene::RemoveSystem( MyUUID ID )
{
    bool Check = HasSystem( ID );

    if ( Check )
    {
        auto ITR = m_SystemIDData.find( ID );
        m_SystemIDData.erase( ITR );

        UpdateSequence();
    }
}

bool Scene::HasSystem( MyUUID ID )
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
    auto NodeITR = m_NodeTypeData.find( Type );
    if ( NodeITR == m_NodeTypeData.end() ) return ;

    auto SystemITR = m_PendingSystemIDData.find( Type );
    if ( SystemITR == m_PendingSystemIDData.end() ) return;

    for ( auto SystemID : m_PendingSystemIDData[ Type ] )
    {
        m_SystemIDData.insert( SystemID );
    }

    m_PendingSystemIDData.erase( SystemITR );

    UpdateSequence();
}

void Scene::ClearSequence( ISystemQueue& Sequence )
{
    while( !Sequence.empty() ) { Sequence.pop(); }
}

void Scene::UpdateSequence()
{
    SystemManager::GetHandle().UpdateSequence( m_SystemIDData, m_MainSequence );
    ClearSequence( m_DoneSequence );
}

Scene::TypeUnSet& Scene::GetNodeTypeData() { return m_NodeTypeData; }
MyUUIDUnSet& Scene::GetSystemIDData() { return m_SystemIDData; }

std::string Scene::GetName() { return m_Name; }
int Scene::GetIndex() { return m_Index; }

void Scene::SetName( std::string Name ) { m_Name = Name; }
void Scene::SetIndex( int Index ) { m_Index = Index; }

