#include "Scene.h"

#include "SystemManager.h"

Scene::Scene() {}
Scene::Scene( std::string Name ) : m_Name( Name ) {}
Scene::Scene( int Index ) : m_Index( Index ) {}
Scene::Scene( int Index, std::string Name ) : m_Index( Index ), m_Name( Name ) {}
Scene::~Scene() {}

void Scene::Init()
{

}

void Scene::Destroy()
{
    m_EntityID.clear();
    m_SystemID.clear();
}

void Scene::Update( float DeltaTime )
{
    while( !m_Sequence.empty() )
    {
        ISystem* System = m_Sequence.front();

        System->Update( DeltaTime );

        m_Sequence.pop();
        m_Done.push( System );
    }

    while( !m_Done.empty() )
    {
        m_Sequence.push( m_Done.front() );
        m_Done.pop();
    }
}

void Scene::RegisterEntity( MyUUID ID )
{
    bool Check = HasEntity( ID );

    if ( !Check ) m_EntityID.insert( ID );
}

void Scene::RemoveEntity( MyUUID ID )
{
    bool Check = HasEntity( ID );

    if ( Check )
    {
        auto ITR = m_EntityID.find( ID );
        m_EntityID.erase( ID );
    }
}

bool Scene::HasEntity( MyUUID ID )
{
    auto ITR = m_EntityID.find( ID );

    if ( ITR != m_EntityID.end() ) return true;
    else return false;
}

void Scene::RegisterSystem( MyUUID ID )
{
    bool Check = HasSystem( ID );

    if ( !Check ) m_SystemID.insert( ID );

    SystemManager::GetHandle().UpdateSequence( m_SystemID, m_Sequence );
}

void Scene::RemoveSystem( MyUUID ID )
{
    bool Check = HasSystem( ID );

    if ( Check )
    {
        auto ITR = m_SystemID.find( ID );
        m_SystemID.erase( ID );

        SystemManager::GetHandle().UpdateSequence( m_SystemID, m_Sequence );
    }
}

bool Scene::HasSystem( MyUUID ID )
{
    auto ITR = m_SystemID.find( ID );

    if ( ITR != m_SystemID.end() ) return true;
    else return false;
}

Scene::EntityID& Scene::GetEntityIDs() { return m_EntityID; }
Scene::SystemID& Scene::GetSystemIDs() { return m_SystemID; }
std::string Scene::GetName() { return m_Name; }
int Scene::GetIndex() { return m_Index; }

void Scene::SetName( std::string Name ) { m_Name = Name; }
void Scene::SetIndex( int Index ) { m_Index = Index; }

