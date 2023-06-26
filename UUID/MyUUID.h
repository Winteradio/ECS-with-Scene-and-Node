#ifndef __MYUUID_H__
#define __MYUUID_H__

#include "Utils.h"

#include <LogProject/Log.h>

class MyUUID
{
	public :
		MyUUID();
		MyUUID( std::string ID );
		MyUUID( const MyUUID& Other );
		bool operator == ( const MyUUID& Other ) const;
		bool operator < (const MyUUID& Other) const;
		MyUUID& operator = ( const std::string& ID );
		~MyUUID();

	public :
		void Init();
		std::string& GetString();
		bool Empty();

	private :
		std::string m_ID;
};

class MyUUIDCompare
{
	public :
		bool operator() ( MyUUID Id1, MyUUID Id2 ) const
		{
			if ( Id1.GetString() < Id2.GetString() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
};

template< typename T >
using MyUUIDMap = std::map< MyUUID, T >;

using MyUUIDSet = std::set< MyUUID >;
using MyUUIDQueue = std::queue< MyUUID >;
using MyUUIDVector = std::vector< MyUUID >;

#endif // __MYUUID_H__