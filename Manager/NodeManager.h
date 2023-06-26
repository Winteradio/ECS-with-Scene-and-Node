#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include "INode.h"
#include "Entity.h"

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>

class NodeManager
{
    using pINodeMap = MyUUIDMap< INode* >;
    using TypepINodeMap = std::map< const std::type_info*, pINodeMap >;

    private :
        NodeManager();
        ~NodeManager();

    public :
        static NodeManager& GetHandle();

        void Init();
        void Destroy();

        TypepINodeMap& GetData();

    public :
        template< typename T >
        T* Create( Entity& Object )
        {
            bool Check = HasMap<T>();
            if ( Check )
            {
                Check = HasNode<T>( Object.GetID() );

                if ( Check )
                {
                    Log::Warn( "Alreay Object has Node");
                    return nullptr;
                }
            }
            else
            {
                CreateMap<T>();
            }

            T* pNode = MemoryManager::GetHandle().Allocate<T>();
            
            Check = pNode->Init( Object );

            if ( Check )
            {
                GetMap<T>()[ Object.GetID() ] = pNode;
                return pNode;
            }
            else
            {
                MemoryManager::GetHandle().Deallocate<T>( pNode );
                Log::Warn( " Invalid Entity for this Node " );
                return nullptr;
            }
        }

        template< typename T >
        void Remove( MyUUID ID )
        {
            bool Check = HasNode<T>( ID );

            if ( Check )
            {
                auto ITR = GetMap<T>().find( ID );
                GetMap<T>().erase( ITR );
            }
        }

        template< typename T >
        T* GetNode( MyUUID ID )
        {
            bool Check = HasNode<T>( ID );
            if ( Check )
            {
                T* Node = dynamic_cast<T*>( GetMap<T>().[ ID ] );

                if ( Node ) return Node;
                else
                {
                    Log::Warn( " Invalid Node Type " );
                }
            }
            else
            {
                Log::Warn( " Node not found of %s ", ID.GetString().c_str() );
            }
        }

        template< typename T >
        bool HasNode( MyUUID ID )
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
            m_Data[ &typeid( T ) ] = pINodeMap();

            m_Data[ &typeid( T ) ].clear();
        }

        template< typename T >
        pINodeMap& GetMap()
        {
            return m_Data[ &typeid( T ) ];
        }

    private :
        static NodeManager m_NodeManager;
        TypepINodeMap m_Data;
};

#endif // __NODEMANAGER_H__