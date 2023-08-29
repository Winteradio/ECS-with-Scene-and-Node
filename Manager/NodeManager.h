#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include "INode.h"
#include "Entity.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class NodeManager
{
    using MyUUIDINodeMPtrUnMap = MyUUIDUnMap< MemoryPtr<INode> >;
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
            bool Check = HasIDSet<T>();
            if ( !Check ) CreateIDSet<T>();

            MemoryPtr<T> NodeMPtr = MemoryManager::GetHandle().Create<T>( Object->GetID() );
            
            Check = NodeMPtr->Check( Object );

            if ( !Check ) 
            { 
                MemoryManager::GetHandle().Delete<T>( NodeMPtr );
                throw Except( " NodeManager | %s | This Entity has not components for this node ", __FUNCTION__, typeid( T ).name() );
            }

            NodeMPtr->Init( Object );
            GetIDData<T>().insert( NodeMPtr->GetID() );
            m_INodeMPtrUnMap[ NodeMPtr->GetID() ] = NodeMPtr;

            return NodeMPtr;
        }

        template< typename T >
        void Remove( MyUUID ID )
        {
            bool Check = HasNode<T>( ID );

            if ( Check )
            {
                MemoryManager::GetHandle().Delete<T>( GetNode( ID ) );
                
                auto ITR = GetIDData<T>().find( ID );
                GetIDData<T>().erase( ITR );
            }
        }

        template< typename T >
        MemoryPtr<T> GetNode( MyUUID ID )
        {
            bool Check = HasNode<T>( ID );

            if ( ! Check )
            {
                throw Except( " NodeManager | %s | %s | There is no Node for %s ID ", __FUNCTION__, typeid( T ).name(), ID.GetString().c_str() );
            }

            return m_INodeMPtrUnMap[ ID ];
        }

        template< typename T >
        bool HasNode( MyUUID ID )
        {
            bool Check = HasIDSet<T>();
            if ( !Check ) CreateIDSet<T>();

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
        bool HasIDSet() { return HasIDSet( &typeid( T ) ); }

        template< typename T >
        void CreateIDSet() { CreateIDSet( &typeid( T ) ); }

    private :
        bool HasIDSet( const std::type_info* Type );
        void CreateIDSet( const std::type_info* Type );

    private :
        static NodeManager m_NodeManager;
        MyUUIDINodeMPtrUnMap m_INodeMPtrUnMap;
        TypeMyUUIDUnSetUnMap m_INodeIDData;
        TypeUnSet m_INodeTypeData;
};

#endif // __NODEMANAGER_H__