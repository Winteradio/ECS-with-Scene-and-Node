#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>
#include "IComponent.h"

class ComponentManager
{
    private :
        ComponentManager();
        ~ComponentManager();

    public :
        static ComponentManager& GetHandle();

        void Init();
        void Destroy();

    private :
        static ComponentManager m_ComponentManager;
};

#endif // __COMPONENTMANAGER_H__