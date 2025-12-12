#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "IComponent.h"

#include <LogProject/Log.h>

class Entity : public IObject
{
	using TypeUnSet = std::unordered_set< const std::type_info* >;
	using TypeMyUUIDUnMap = std::unordered_map< const std::type_info*, MyUUID >;

    public :
        Entity();
        Entity( std::string Name );
        Entity( MyUUID ID );
        Entity( MyUUID ID, std::string Name );
        virtual ~Entity();

		template< typename T >
		void AddComponent( MyUUID ID ) { return AddComponent( &typeid( T ), ID ); }

		template< typename T >
		void RemoveComponent() { return RemoveComponent( &typeid( T ) ); }

		template< typename T >
		MyUUID GetComponent() { return GetComponent( &typeid( T ) ); }

		template< typename T >
		bool HasComponent() { return HasComponent( &typeid( T ) ); }

	public :
		void AddComponent( const std::type_info* Type, MyUUID ID );
		void RemoveComponent( const std::type_info* Type );
		MyUUID GetComponent( const std::type_info* Type );
		bool HasComponent( const std::type_info* Type );

	public :
		TypeUnSet GetTypeData();
		TypeMyUUIDUnMap& GetIDData();
		std::string& GetName();

		void SetName( std::string Name );

	private :
		TypeMyUUIDUnMap m_ComponentIDData;
		std::string m_Name;
};

#endif // __ENTITY_H__