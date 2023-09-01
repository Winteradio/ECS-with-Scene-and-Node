#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Scene.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class SceneManager
{
    using IndexSet = std::set< int >;
    using IndexSceneMPtrUnMap = std::unordered_map< int, MemoryPtr<Scene> >;

    private :
        SceneManager();
        ~SceneManager();

    public :
        static SceneManager& GetHandle();

        void Init();
        void Destroy();

    public :
        MemoryPtr<Scene> Create();
        MemoryPtr<Scene> Create( std::string Name );

        void Remove( int& Index );

        MemoryPtr<Scene> GetScene( int& Index );

        IndexSet& GetIndexData();
        size_t GetCount();

    private :
        bool HasScene( int& Index );

    private :
        static SceneManager m_SceneManager;
        IndexSceneMPtrUnMap m_SceneMPtrData;
        IndexSet m_SceneIndexData;
};

#endif // __SCENEMANAGER_H__