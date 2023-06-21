#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include <MemoryManager.h>
#include "IComponent.h"

class Core;

class ComponentManager
{
    public :
        ComponentManager();
        ~ComponentManager();

        friend class Core;

    public :
        void Init();
        void Destroy();

    private :
};

#endif // __COMPONENTMANAGER_H__