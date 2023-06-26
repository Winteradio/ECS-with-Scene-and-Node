#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include "IComponent.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class ComponentManager
{
    using pIComponentMap = MyUUIDMap< IComponent* >;
    using TypepIComponentMap = std::map< const std::type_info*, pIComponentMap >;

    private :
        ComponentManager();
        ~ComponentManager();

    public :
        static ComponentManager& GetHandle();

        void Init();
        void Destroy();

        TypepIComponentMap& GetData();

    public :
        template< typename T, typename... Args >
        T* Create( Args&&... args )
        {
            bool Check = HasMap<T>();
            
            if ( !Check ) CreateMap<T>();

            T* Component = MemoryManager::GetHandle().Allocate<T>( std::forward<Args>(args)... );
            GetMap<T>()[ Component->GetID() ] = Component;

            return Component;
        }

        template< typename T >
        void Remove( MyUUID ID )
        {
            bool Check = HasComponent<T>( ID );

            if ( Check )
            {
                auto ITR = GetMap<T>().find( ID );
                GetMap<T>().erase( ITR );
            }
        }

        template< typename T >
        T* GetComponent( MyUUID ID )
        {
            bool Check = HasComponent<T>( ID );
            if ( Check )
            {
                T* pComponent = dynamic_cast<T*>( GetMap<T>().[ ID ] );

                if ( pComponent ) return pComponent;
                else
                {
                    Log::Warn( " Invalid Component Type " );
                }
            }
            else
            {
                Log::Warn( " Component not found of %s ", ID.GetString().c_str() );
            }
        }

        template< typename T >
        bool HasComponent( MyUUID ID )
        {
            bool Check = HasMap<T>();
            if ( !Check ) return false;

            auto ITR = GetMap<T>().find( ID );

            if ( ITR != GetMap<T>().end() )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        template< typename T >
        bool HasMap()
        {
            if ( m_Data.find( &typeid( T ) ) != m_Data.end() ) 
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        template< typename T >
        void CreateMap()
        {
            m_Data[ &typeid( T ) ] = pIComponentMap();

            m_Data[ &typeid( T ) ].clear();
        }

        template< typename T >
        pIComponentMap& GetMap()
        {
            return m_Data[ &typeid( T ) ];
        }

    private :
        static ComponentManager m_ComponentManager;
        TypepIComponentMap m_Data;
};

#endif // __COMPONENTMANAGER_H__