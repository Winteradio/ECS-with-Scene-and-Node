#ifndef __MYUUID_H__
#define __MYUUID_H__

#include <string>
#include <Rpc.h>
#include "LogProject/Log.h"
#include <set>
#include <map>

class MyUUID
{
	public :
		MyUUID();
		MyUUID( std::string ID );
		MyUUID( const MyUUID& Other );
		bool operator == ( const MyUUID& Other ) const;
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

using IDSet = std::set< MyUUID, MyUUIDCompare >;

template< typename T >
using IDMap = std::map< MyUUID, T, MyUUIDCompare >;

#endif // __MYUUID_H__