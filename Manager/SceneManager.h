#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>
#include "IScene.h"

class SceneManager
{
    private :
        SceneManager();
        ~SceneManager();

    public :
        static SceneManager& GetHandle();

        void Init();
        void Destroy();

    private :
        static SceneManager m_SceneManager;
};

#endif // __SCENEMANAGER_H__