#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Scene.h"

#include <LogProject/Log.h>

class SceneManager
{
    using pSceneMap = std::map< int, Scene* >;

    private :
        SceneManager();
        ~SceneManager();

    public :
        static SceneManager& GetHandle();

        void Init();
        void Destroy();

    public :
        Scene* Create();
        Scene* Create( std::string Name );

        void Remove( int Index );

        bool HasScene( int Index );
        Scene* GetScene( int Index );

        pSceneMap& GetData();
        size_t GetCount();

    private :
        static SceneManager m_SceneManager;
        pSceneMap m_Data;
};

#endif // __SCENEMANAGER_H__