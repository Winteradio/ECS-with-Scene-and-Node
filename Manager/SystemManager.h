#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMNANAGER_H__

#include "ISystem.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class SystemManager
{
    using pISystemMap = MyUUIDMap< ISystem* >;
    using DependencyIDSetMap = std::map< MyUUID, MyUUIDSet >;
    using DependencyIndegreeMap = std::map< MyUUID, int >;

    using SystemSequence = std::queue< ISystem* >;

    private :
        SystemManager();
        ~SystemManager();

    public :
        static SystemManager& GetHandle();

        void Init();
        void Destroy();

    public :
        template< typename T >
        ISystem* Create()
        {
            MyUUID ID;
            ID.Init();

            return Create<T>( ID );
        }

        template< typename T >
        ISystem* Create( MyUUID ID )
        {
            bool Check = HasSystem( ID );
            
            if ( Check )
            {
                Log::Warn( " Alreay the System is existed " );
                return nullptr;
            }
            else
            {
                T* System = new T( ID );

                m_Data[ ID ] = System;
                m_DependencyIDSetMap[ ID ] = MyUUIDSet();

                return System;
            }
        }

    public :
        ISystem* GetSystem( MyUUID ID );
        bool HasSystem( MyUUID ID );
        void Remove( MyUUID ID );

        void SetDependency( MyUUID MainID, MyUUID DependencyID );
        void DeleteDependency( MyUUID MainID, MyUUID DependencyID );

        void UpdateSequence( MyUUIDSet& SceneSystems, SystemSequence& SceneSequence );

    private :
        bool TopologySort( MyUUIDSet& SceneSystems, SystemSequence& SceneSequence = SystemSequence() );
        DependencyIndegreeMap CalculateIndegree( DependencyIDSetMap& DepIDSetMap );

        DependencyIDSetMap CalculateDependency( MyUUIDSet& SceneSystems );
        void RecursiveCheckDependency( MyUUID MainID, MyUUID SystemID, MyUUIDSet& SceneSystems, DependencyIDSetMap& DepIDSetMap );

    private :
        static SystemManager m_SystemManager;
        pISystemMap m_Data;
        DependencyIDSetMap m_DependencyIDSetMap;
};

#endif // __SYSTEMMANAGER_H__