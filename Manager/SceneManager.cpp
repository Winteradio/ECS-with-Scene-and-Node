#include "SceneManager.h"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::Init()
{
    Log::Info(" Initialize Scene Manager ");
}

void SceneManager::Destroy()
{
    m_SceneMPtrUnMap.clear();
    m_SceneIndexSet.clear();
    
    Log::Info(" Destroy Scene Manager ");
}

MemoryPtr<Scene> SceneManager::Create()
{
    return Create( "Default" );
}

MemoryPtr<Scene> SceneManager::Create( std::string Name )
{
    int Index = static_cast<int>( GetCount() );

    bool Check = HasScene( Index );

    if ( Check )
    {
        throw Except(" SceneManager | %s | %s | The index %d was already existed ", __FUNCTION__, typeid( Scene ).name(), Index );
    }

    m_SceneIndexSet.insert( Index );
    m_SceneMPtrUnMap[ Index ] = MemoryManager::GetHandle().Create<Scene>( Index, Name );
    
    return GetScene( Index );
}

void SceneManager::Remove( int Index )
{
    bool Check = HasScene( Index );

    if ( Check )
    {
        MemoryManager::GetHandle().Delete<Scene>( GetScene( Index ) );

        {
            auto ITR = m_SceneIndexSet.find( Index );
            m_SceneIndexSet.erase( ITR );
        }

        {
            auto ITR = m_SceneMPtrUnMap.find( Index );
            m_SceneMPtrUnMap.erase( ITR );
        }
    }
}

bool SceneManager::HasScene( int Index )
{
    auto ITR = m_SceneIndexSet.find( Index );

    if ( ITR != m_SceneIndexSet.end() ) return true;
    else return false;
}

MemoryPtr<Scene> SceneManager::GetScene( int Index ) 
{ 
    bool Check = HasScene( Index );

    if ( !Check ) 
    {
        throw Except(" SceneManager | %s | %s | The index was not registerd ", __FUNCTION__, typeid( Scene ).name() );
    }

    return m_SceneMPtrUnMap[ Index ];
}

SceneManager::IndexSet& SceneManager::GetIndexData() { return m_SceneIndexSet; }
size_t SceneManager::GetCount() { return m_SceneMPtrUnMap.size(); }
SceneManager& SceneManager::GetHandle() { return m_SceneManager; }

SceneManager SceneManager::m_SceneManager;