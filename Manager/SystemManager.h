#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMNANAGER_H__

#include <MemoryProject/MemoryManager.h>
#include "ISystem.h"

#include <memory>
#include <set>
#include "MyUUID.h"

class SystemManager
{
    using SceneID = MyUUID;
    using SystemID = MyUUID;

    using Data = IDMap< ISystem* >;
    using Need = std::map< SystemID, IDSet, MyUUIDCompare >;
    using Sequence = std::map< SceneID, std::queue< ISystem* >, MyUUIDCompare >;
    using Done = std::map< SceneID, std::queue< ISystem* >, MyUUIDCompare >;
    using SystemQueue = std::queue< std::shared_ptr< ISystem > >;
    
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

            Create<T>( ID );
        }
        
        template< typename T >
        ISystem* Create( MyUUID ID )
        {
            ISystem*& System = MemoryManager::GetHandle().Allocate< T >();
        }

        void SetDependency( ISystem*& Main, ISystem*& Dependency );
        void DeleteDependency( ISystem*& Main, ISystem*& Dependency );

        bool Sort( MyUUID SceneID );

        SystemQueue& GetSequence( MyUUID SceneID );

    private :
        static SystemManager m_SystemManager;
};

#endif // __SYSTEMMANAGER_H__