#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>
#include "IEntity.h"

class EntityManager
{
    private :
        EntityManager();
        ~EntityManager();

    public :
        static EntityManager& GetHandle();

        void Init();
        void Destroy();

    private :
        static EntityManager m_EntityManager;
};

#endif // __ENTITYMANAGER_H__