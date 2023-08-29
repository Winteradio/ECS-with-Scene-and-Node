#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "IComponent.h"

#include <LogProject/Log.h>

class Entity : public IObject
{
	using TypeMyUUIDUnMap = std::unordered_map< const std::type_info*, MyUUID >;

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
			if ( !Result ) m_ComponentID[ &typeid( T ) ] = ID;
		}

		template< typename T >
		void RemoveComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result ) m_ComponentID.erase( &typeid( T ) );
		}

		template< typename T >
		MyUUID GetComponent()
		{
			bool Result = HasComponent<T>();

			if ( !Result )
			{
				throw Except( " Entity | %s | This Entity has not type of %s component ", __FUNCTION__, typeid( T ).name() );
			}

			return m_ComponentID[ &typeid( T ) ];
		}

		template< typename T >
		bool HasComponent()
		{
			auto ITR = m_ComponentID.find( &typeid( T ) );
			if ( ITR != m_ComponentID.end() ) { return true; }
			else { return false; }
		}

	public :
		void SetName( std::string Name );

		TypeMyUUIDUnMap& GetIDData();
		std::string& GetName();

	private :
		TypeMyUUIDUnMap m_ComponentID;
		std::string m_Name;
};

#endif // __ENTITY_H__