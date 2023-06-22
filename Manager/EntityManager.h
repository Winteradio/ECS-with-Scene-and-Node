#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <MemoryProject/MemoryManager.h>
#include "IEntity.h"

class EntityManager
{
    public :
        EntityManager();
        ~EntityManager();

    public :
        void Init();
        void Destroy();

    private :
};

#endif // __ENTITYMANAGER_H__