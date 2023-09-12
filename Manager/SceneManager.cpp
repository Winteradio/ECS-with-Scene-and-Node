#include "SceneManager.h"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::Init()
{
    m_SceneMPtrData.clear();
    m_SceneIndexData.clear();
    
    Log::Info(" Initialize Scene Manager ");
}

void SceneManager::Destroy()
{
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

    m_SceneIndexData.insert( Index );
    m_SceneMPtrData[ Index ] = MemoryManager::GetHandle().Create<Scene>( Index, Name );
    
    return GetScene( Index );
}

void SceneManager::Remove( int& Index )
{
    bool Check = HasScene( Index );

    if ( Check )
    {
        MemoryManager::GetHandle().Delete<Scene>( GetScene( Index ) );

        auto IndexITR = m_SceneIndexData.find( Index );
        if ( IndexITR != m_SceneIndexData.end() ) m_SceneIndexData.erase( IndexITR );

        auto MPtrITR = m_SceneMPtrData.find( Index );
        if ( MPtrITR != m_SceneMPtrData.end() ) m_SceneMPtrData.erase( MPtrITR );
    }
}

bool SceneManager::HasScene( int& Index )
{
    auto ITR = m_SceneIndexData.find( Index );
    if ( ITR != m_SceneIndexData.end() ) return true;
    else return false;
}

MemoryPtr<Scene> SceneManager::GetScene( int& Index ) 
{ 
    bool Check = HasScene( Index );

    if ( !Check ) 
    {
        throw Except(" SceneManager | %s | %s | The index was not registerd ", __FUNCTION__, typeid( Scene ).name() );
    }

    return m_SceneMPtrData[ Index ];
}

SceneManager::IndexSet& SceneManager::GetIndexData() { return m_SceneIndexData; }
size_t SceneManager::GetCount() { return m_SceneMPtrData.size(); }
SceneManager& SceneManager::GetHandle() { return m_SceneManager; }

SceneManager SceneManager::m_SceneManager;