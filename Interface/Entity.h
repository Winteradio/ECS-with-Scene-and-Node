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
			if ( !Result ) m_ComponentIDData[ &typeid( T ) ] = ID;
		}

		template< typename T >
		void RemoveComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result ) m_ComponentIDData.erase( &typeid( T ) );
		}

		template< typename T >
		MyUUID GetComponent()
		{
			bool Result = HasComponent<T>();

			if ( !Result )
			{
				throw Except( " Entity | %s | This Entity has not type of %s component ", __FUNCTION__, typeid( T ).name() );
			}

			return m_ComponentIDData[ &typeid( T ) ];
		}

		template< typename T >
		bool HasComponent()
		{
			auto ITR = m_ComponentIDData.find( &typeid( T ) );
			if ( ITR != m_ComponentIDData.end() ) { return true; }
			else { return false; }
		}

	public :
		void SetName( std::string Name );

		TypeMyUUIDUnMap& GetIDData();
		std::string& GetName();

	private :
		TypeMyUUIDUnMap m_ComponentIDData;
		std::string m_Name;
};

#endif // __ENTITY_H__