#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include <MemoryProject/MemoryManager.h>
#include "IScene.h"

class SceneManager
{
    public :
        SceneManager();
        ~SceneManager();

    public :
        void Init();
        void Destroy();

    private :
};

#endif // __SCENEMANAGER_H__