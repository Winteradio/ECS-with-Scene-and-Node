#ifndef __MYUUID_H__
#define __MYUUID_H__

#include <LogProject/Log.h>
#include "Utils.h"

class MyUUID
{
	public :
		MyUUID();
		MyUUID( std::string ID );
		MyUUID( const MyUUID& Other );
		bool operator == ( const MyUUID& Other ) const;
		bool operator != ( const MyUUID& Other ) const;
		bool operator < (const MyUUID& Other) const;
		MyUUID& operator = ( const std::string& ID );
		~MyUUID();

	public :
		void Init();
		std::string& GetString();
		std::string GetString() const;
		bool Empty();

	private :
		std::string m_ID;

	public :
		class HashFunction
		{
			public :
				std::size_t operator()( const MyUUID& ID ) const
				{
					return std::hash< std::string >()( ID.GetString() );
				}
		};
};

template< typename T >
using MyUUIDUnMap = std::unordered_map< MyUUID, T, MyUUID::HashFunction >;

template< typename T >
using MyUUIDMap = std::map< MyUUID, T >;

using MyUUIDSet = std::set< MyUUID >;
using MyUUIDUnSet = std::unordered_set< MyUUID, MyUUID::HashFunction >;

using MyUUIDQueue = std::queue< MyUUID >;
using MyUUIDVector = std::vector< MyUUID >;

#endif // __MYUUID_H__