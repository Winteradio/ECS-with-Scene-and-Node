#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class EntityManager
{
    using pEntityMap = MyUUIDMap< Entity* >;

    private :
        EntityManager();
        ~EntityManager();

    public :
        static EntityManager& GetHandle();

        void Init();
        void Destroy();

        Entity* Create();
        Entity* Create( std::string Name );
        Entity* Create( MyUUID ID );
        Entity* Create( MyUUID ID, std::string Name );
        
        void Remove( MyUUID ID );
        bool HasEntity( MyUUID ID );

        Entity* GetEntity( MyUUID ID );
        pEntityMap& GetData();

    private :
        static EntityManager m_EntityManager;
        pEntityMap m_Data;
};

#endif // __ENTITYMANAGER_H__