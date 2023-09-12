#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMNANAGER_H__

#include "ISystem.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class SystemManager
{
    using MyUUIDISystemMPtrUnMap = MyUUIDUnMap< MemoryPtr<ISystem> >;
    using DependencyIDUnSetUnMap = MyUUIDUnMap< MyUUIDUnSet >;
    using DependentIndegreeUnMap = MyUUIDUnMap< int >;
    using SystemIndexUnMap = MyUUIDUnMap< int >;

    private :
        SystemManager();
        ~SystemManager();

    public :
        static SystemManager& GetHandle();

        void Init();
        void Destroy();
        void Clear();

    public :
        template< typename T >
        MemoryPtr<ISystem> Create()
        {
            MyUUID ID;

            return Create<T>( ID );
        }

        template< typename T >
        MemoryPtr<ISystem> Create( MyUUID ID )
        {
            bool Check = HasSystem( ID );
            if ( Check ) { throw Except( " SystemManager | %s | This System is already existed ", typeid( T ).name() ); }
        
            MemoryPtr<ISystem> System = MemoryManager::GetHandle().CreateOne<T>( ID );

            m_ISystemMPtrData[ ID ] = System;
            m_DependencyIDData[ ID ] = MyUUIDUnSet();

            return System;
        }

    public :
        MemoryPtr<ISystem> GetSystem( MyUUID& ID );
        bool HasSystem( MyUUID& ID );
        void Remove( MyUUID& ID );

        void SetDependency( MyUUID& MainID, MyUUID& DependentID );
        void DeleteDependency( MyUUID& MainID, MyUUID& DependentID );

        MyUUIDVector UpdateSequence( MyUUIDUnSet& SystemIDData );

    private :
        bool TopologySort( MyUUIDUnSet& SystemIDData );
        DependentIndegreeUnMap CalculateIndegree( DependencyIDUnSetUnMap& DependencyIDData );
        
    private :
        static SystemManager m_SystemManager;
        MyUUIDISystemMPtrUnMap m_ISystemMPtrData;
        DependencyIDUnSetUnMap m_DependencyIDData;
        SystemIndexUnMap m_SystemIndexData;
};

#endif // __SYSTEMMANAGER_H__