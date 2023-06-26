#include "SceneManager.h"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::Init()
{
    Log::Info(" Initialize Scene Manager ");
}

void SceneManager::Destroy()
{
    Log::Info(" Destroy Scene Manager ");
}

Scene* SceneManager::Create()
{
    return Create( "Default" );
}

Scene* SceneManager::Create( std::string Name )
{
    int Index = static_cast<int>( GetCount() ); 
    m_Data[ Index ] = new Scene( Index, Name );

    return m_Data[ Index ];
}

void SceneManager::Remove( int Index )
{
    bool Check = HasScene( Index );
    if ( Check )
    {
        delete GetScene( Index );

        auto ITR = m_Data.find( Index );
        m_Data.erase( ITR );
    }
}

bool SceneManager::HasScene( int Index )
{
    if ( Index < static_cast<int>( GetCount() ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

Scene* SceneManager::GetScene( int Index ) 
{ 
    bool Check = HasScene( Index );
    if ( Check )
    {
        return m_Data[ Index ];
    }
    else
    {
        Log::Warn(" There is no Scene for Index %d ", Index );
        return nullptr;
    }
}

SceneManager::pSceneMap& SceneManager::GetData() { return m_Data; }
size_t SceneManager::GetCount() { return m_Data.size(); }

SceneManager& SceneManager::GetHandle() { return m_SceneManager; }
SceneManager SceneManager::m_SceneManager;