#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include "IComponent.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class ComponentManager
{
    private :
        ComponentManager();
        ~ComponentManager();

    public :
        static ComponentManager& GetHandle();

        void Init();
        void Destroy();
        void Clear();

    public :
        template< typename T, typename... Args >
        MemoryPtr<T> Create( Args&&... args )
        {
            bool Check = HasIDSet<T>();
            if ( !Check ) CreateIDSet<T>();

            MyUUID ID;
            ID.Init();
            
            MemoryPtr<T> ComponentMPtr = MemoryManager::GetHandle().Create<T>( std::forward<Args>(args)... );

            GetIDData<T>().insert( ComponentMPtr->GetID() );
            m_IComponentMPtrData[ ComponentMPtr->GetID() ] = ComponentMPtr;

            return ComponentMPtr;
        }

        template< typename T >
        void Remove( MyUUID& ID )
        {
            bool Check = HasComponent<T>( ID );

            if ( Check )
            {
                MemoryManager::GetHandle().Delete<T>( GetComponent( ID ) );
                
                auto IDITR = GetIDData<T>().find( ID );
                if ( IDITR != GetIDData<T>().end() ) GetIDData<T>().erase( ITR );

                auto MPtrITR = m_IComponentMPtrData.find( ID );
                if ( MPtrITr != m_IComponentMPtrData.end() ) m_IComponentMPtrData.erase( MPtrITR );
            }
        }

        template< typename T >
        MemoryPtr<T> GetComponent( MyUUID& ID )
        {
            bool Check = HasComponent<T>( ID );

            if ( !Check )
            {
                throw Except( " ComponentManager | %s | %s | There is no Component for %s ID ", __FUNCTION__, typeid( T ).name(), ID.GetString().c_str() );
            }

            return m_IComponentMPtrData[ ID ];
        }

        template< typename T >
        bool HasComponent( MyUUID& ID )
        {
            bool Check = HasIDSet<T>();
            if ( !Check ) return false;

            auto ITR = GetIDData<T>().find( ID );
            if ( ITR != GetIDData<T>().end() ) return true;
            else return false;
        }

        template< typename T >
        MyUUIDUnSet& GetIDData() { return GetIDData( &typeid( T ) ); }

    public :
        TypeUnSet& GetTypeData();
        MyUUIDUnSet& GetIDData( const std::type_info* Type );

    private :
        template< typename T >
        bool HasIDSet() { return HasIDSet( &typeid( T ) ); }

        template< typename T >
        void CreateIDSet() { CreateIDSet( &typeid( T ) ); }

    private :
        bool HasIDSet( const std::type_info* Type );
        void CreateIDSet( const std::type_info* Type );

    private :
        static ComponentManager m_ComponentManager;
        MyUUIDIComponentMPtrUnMap m_IComponentMPtrData;
        TypeMyUUIDUnSetUnMap m_IComponentIDData;
};

#endif // __COMPONENTMANAGER_H__