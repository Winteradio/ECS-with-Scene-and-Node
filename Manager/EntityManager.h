#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class EntityManager
{
    using MyUUIDEntityMPtrUnMap = MyUUIDUnMap< MemoryPtr<Entity> >;

    private :
        EntityManager();
        ~EntityManager();

    public :
        static EntityManager& GetHandle();

        void Init();
        void Destroy();

        MemoryPtr<Entity> Create();
        MemoryPtr<Entity> Create( std::string Name );
        MemoryPtr<Entity> Create( MyUUID ID );
        MemoryPtr<Entity> Create( MyUUID ID, std::string Name );
        
        void Remove( MyUUID ID );
        
        MemoryPtr<Entity> GetEntity( MyUUID ID );

        MyUUIDUnSet& GetIDData();

    private :
        bool HasEntity( MyUUID ID );

    private :
        static EntityManager m_EntityManager;
        MyUUIDEntityMPtrUnMap m_EntityMPtrUnMap;
        MyUUIDUnSet m_EntityIDUnSet;
};

#endif // __ENTITYMANAGER_H__