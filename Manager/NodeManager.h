#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include "INode.h"
#include "Entity.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class NodeManager
{
    using MyUUIDINodeMPtrUnMap = MyUUIDUnMap< MemoryPtr<INode> >;
    using TypeNodeMPtrUnMap = std::unordered_map< const std::type_info*, MyUUIDINodeMPtrUnMap >;
    using TypeMyUUIDUnSetUnMap = std::unordered_map< const std::type_info*, MyUUIDUnSet >;
    using TypeUnSet = std::unordered_set< const std::type_info* >;

    private :
        NodeManager();
        ~NodeManager();

    public :
        static NodeManager& GetHandle();

        void Init();
        void Destroy();

    public :
        template< typename T >
        MemoryPtr<T> Create( MemoryPtr<Entity>& Object )
        {
            bool Check = HasMPtrMap<T>();
            if ( !Check ) CreateMPtrMap<T>();

            MemoryPtr<T> NodeMPtr = MemoryManager::GetHandle().Create<T>( Object->GetID() );
            
            Check = NodeMPtr->Check( Object );

            if ( !Check ) 
            { 
                MemoryManager::GetHandle().Delete<T>( NodeMPtr );
                throw Except( " NodeManager | %s | This Entity has not components for this node ", __FUNCTION__, typeid( T ).name() );
            }

            NodeMPtr->Init( Object );
            GetIDData<T>().insert( NodeMPtr->GetID() );
            GetMPtrMapData<T>()[ NodeMPtr->GetID() ] = NodeMPtr;

            return NodeMPtr;
        }

        template< typename T >
        void Remove( MyUUID& ID )
        {
            bool Check = HasNode<T>( ID );

            if ( Check )
            {
                MemoryManager::GetHandle().Delete<T>( GetNode( ID ) );
                
                auto IDITR = GetIDData<T>().find( ID );
                if ( IDITR != GetIDData<T>().end() ) GetIDData<T>().erase( IDITR );

                auto MPtrITR = GetMPtrMapData<T>().find( ID );
                if ( MPtrITR != GetMPtrMapData<T>().end() ) GetMPtrMapData<T>().erase( MPtrITR );
            }
        }

        template< typename T >
        MemoryPtr<T> GetNode( MyUUID& ID )
        {
            bool Check = HasNode<T>( ID );

            if ( ! Check )
            {
                throw Except( " NodeManager | %s | %s | There is no Node for %s ID ", __FUNCTION__, typeid( T ).name(), ID.GetString().c_str() );
            }

            return GetMPtrMapData<T>()[ ID ];
        }

        template< typename T >
        bool HasNode( MyUUID& ID )
        {
            bool Check = HasMPtrMap<T>();
            if ( !Check ) return false;

            auto ITR = GetIDData<T>().find( ID );
            if ( ITR != GetIDData<T>().end() ) return true;
            else return false;
        }

        template< typename T >
        MyUUIDUnSet& GetIDData() { return GetIDData( &typeid( T ) ); }

    public :
        TypeUnSet& GetTypeData();
        MyUUIDUnSet& GetIDData( const std::type_info* TypeID );

    private :
        template< typename T >
        MyUUIDINodeMPtrUnMap& GetMPtrMapData() { return GetMPtrMapData( &typeid( T ) ); }

        template< typename T > 
        bool HasMPtrMap() { return HasMPtrMap( &typeid( T ) ); }

        template< typename T >
        void CreateMPtrMap() { return CreateMPtrMap( &typeid( T ) ); }

    private :
        MyUUIDINodeMPtrUnMap& GetMPtrMapData( const std::type_info* Type );
        bool HasMPtrMap( const std::type_info* Type );
        void CreateMPtrMap( const std::type_info* Type );

    private :
        static NodeManager m_NodeManager;
        TypeNodeMPtrUnMap m_INodeMPtrData;
        TypeMyUUIDUnSetUnMap m_INodeIDData;
        TypeUnSet m_INodeTypeData;
};

#endif // __NODEMANAGER_H__