#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMNANAGER_H__

#include "ISystem.h"

#include <LogProject/Log.h>

class SystemManager
{
    using MyUUIDISystemUnMap = MyUUIDUnMap< ISystem* >;
    using DependencyIDUnSetUnMap = MyUUIDUnMap< MyUUIDUnSet >;
    using DependencyIndegreeUnMap = MyUUIDUnMap< int >;

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

                m_ISystemUnMap[ ID ] = System;
                m_DependencyID[ ID ] = MyUUIDUnSet();

                return System;
            }
        }

    public :
        ISystem* GetSystem( MyUUID ID );
        bool HasSystem( MyUUID ID );
        void Remove( MyUUID ID );

        void SetDependency( MyUUID MainID, MyUUID DependencyID );
        void DeleteDependency( MyUUID MainID, MyUUID DependencyID );

        void UpdateSequence( MyUUIDUnSet& SceneSystems, ISystemQueue& SceneSequence );

    private :
        bool TopologySort( MyUUIDUnSet& SceneSystems, ISystemQueue& SceneSequence = ISystemQueue() );

        DependencyIndegreeUnMap CalculateIndegree( DependencyIDUnSetUnMap& DepIDSetMap );
        DependencyIDUnSetUnMap CalculateDependency( MyUUIDUnSet& SceneSystems );
        
        void RecursiveCheckDependency( MyUUID MainID, MyUUID SystemID, MyUUIDUnSet& SceneSystems, DependencyIDUnSetUnMap& DepIDSetMap );

    private :
        static SystemManager m_SystemManager;
        MyUUIDISystemUnMap m_ISystemUnMap;
        DependencyIDUnSetUnMap m_DependencyID;
};

#endif // __SYSTEMMANAGER_H__