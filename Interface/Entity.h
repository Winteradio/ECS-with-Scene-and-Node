#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "IComponent.h"

#include <LogProject/Log.h>

class Entity : public IObject
{
    using Data = std::map< const std::type_info*, MyUUID >;

    public :
        Entity();
        Entity( std::string Name );
        Entity( MyUUID ID );
        Entity( MyUUID ID, std::string Name );
        virtual ~Entity();

		template< typename T >
		void AddComponent( MyUUID ID )
		{
			bool Result = HasComponent<T>();
			if ( !Result ) m_Data[ &typeid( T ) ] = ID;
		}

		template< typename T >
		void RemoveComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result ) m_Data.erase( &typeid( T ) );
		}

		template< typename T >
		MyUUID* GetComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result ) { return &m_Data[ &typeid( T ) ]; }
			else 
			{ 
				Log::Warn( " Entity has not type of this component" );
				return nullptr; 
			}
		}

		template< typename T >
		bool HasComponent()
		{
			auto itr = m_Data.find( &typeid( T ) );
			if ( itr != m_Data.end() ) { return true; }
			else { return false; }
		}

	public :
		void SetName( std::string Name );

		Data& GetData();
		std::string& GetName();

	private :
		Data m_Data;
		std::string m_Name;
};

#endif // __ENTITY_H__