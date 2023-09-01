#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "ISystem.h"
#include "NodeList.h"
#include "NodeManager.h"

class RenderSystem : public ISystem
{
    public :
        RenderSystem() { SetNodeType(); };
        RenderSystem( MyUUID ID ) : ISystem( ID ) { SetNodeType(); };
        virtual ~RenderSystem() {};

    public :
        virtual void Update( float Deltatime, int SceneIndex, MyUUIDUnSet& NodeIDData )
        {
            for ( auto ID : NodeIDData )
            {
                bool Check = NodeManager::GetHandle().HasNode<RenderNode>( ID );
                if ( Check )
                {
                    MemoryPtr<RenderNode> Node = NodeManager::GetHandle().GetNode<RenderNode>( ID );
                    Log::Info(" RenderSystem Update %s ", ID.GetString().c_str() );
                }
                else
                {
                    Log::Info(" RenderSystem not Update %s ", ID.GetString().c_str() );
                }
            }
        }

        virtual void SetNodeType( const std::type_info* Type = &typeid( RenderNode ) ) { m_NodeType = Type; }
};

class PhysicsSystem : public ISystem
{
    public :
        PhysicsSystem() { SetNodeType(); };
        PhysicsSystem( MyUUID ID ) : ISystem( ID ) { SetNodeType(); };
        virtual ~PhysicsSystem() {};

    public :
        virtual void Update( float Deltatime, int SceneIndex, MyUUIDUnSet& NodeIDData )
        {
            for ( auto ID : NodeIDData )
            {
                bool Check = NodeManager::GetHandle().HasNode<PhysicsNode>( ID );
                if ( Check )
                {
                    MemoryPtr<PhysicsNode> Node = NodeManager::GetHandle().GetNode<PhysicsNode>( ID );
                    Log::Info(" PhysicsSystem Update %s ", ID.GetString().c_str() );
                }
                else
                {
                    Log::Info(" PhysicsSystem not Update %s ", ID.GetString().c_str() );
                }
            }
        }

        virtual void SetNodeType( const std::type_info* Type = &typeid( PhysicsNode ) )
        {
            m_NodeType = Type;
        }
};

class MoveSystem : public ISystem
{
    public :
        MoveSystem() { SetNodeType(); };
        MoveSystem( MyUUID ID ) : ISystem( ID ) { SetNodeType(); };
        virtual ~MoveSystem() {};

    public :
        virtual void Update( float Deltatime, int SceneIndex, MyUUIDUnSet& NodeIDData )
        {
            for ( auto ID : NodeIDData )
            {
                bool Check = NodeManager::GetHandle().HasNode<PhysicsNode>( ID );
                if ( Check )
                {
                    MemoryPtr<PhysicsNode> Node = NodeManager::GetHandle().GetNode<PhysicsNode>( ID );
                    Log::Info(" MoveSystem Update %s ", ID.GetString().c_str() );
                }
                else
                {
                    Log::Info(" MoveSystem not Update %s ", ID.GetString().c_str() );
                }
            }
        }

        virtual void SetNodeType( const std::type_info* Type = &typeid( PhysicsNode ) )
        {
            m_NodeType = Type;
        }
};

class CollisionSystem : public ISystem
{
    public :
        CollisionSystem() { SetNodeType(); };
        CollisionSystem( MyUUID ID ) : ISystem( ID ) { SetNodeType(); };
        virtual ~CollisionSystem() {};

    public :
        virtual void Update( float Deltatime, int SceneIndex, MyUUIDUnSet& NodeIDData )
        {
            for ( auto ID : NodeIDData )
            {
                bool Check = NodeManager::GetHandle().HasNode<CollisionNode>( ID );
                if ( Check )
                {
                    MemoryPtr<CollisionNode> Node = NodeManager::GetHandle().GetNode<CollisionNode>( ID );
                    Log::Info(" CollisionSystem Update %s ", ID.GetString().c_str() );
                }
                else
                {
                    Log::Info(" CollisionSystem not Update %s ", ID.GetString().c_str() );
                }
            }
        }

        virtual void SetNodeType( const std::type_info* Type = &typeid( CollisionNode ) )
        {
            m_NodeType = Type;
        }
};

class CameraSystem : public ISystem
{
    public :
        CameraSystem() { SetNodeType(); };
        CameraSystem( MyUUID ID ) : ISystem( ID ) { SetNodeType(); };
        virtual ~CameraSystem() {};

    public :
        virtual void Update( float Deltatime, int SceneIndex, MyUUIDUnSet& NodeIDData )
        {
            for ( auto ID : NodeIDData )
            {
                bool Check = NodeManager::GetHandle().HasNode<CameraNode>( ID );
                if ( Check )
                {
                    MemoryPtr<CameraNode> Node = NodeManager::GetHandle().GetNode<CameraNode>( ID );
                    Log::Info(" CameraSystem Update %s ", ID.GetString().c_str() );
                }
                else
                {
                    Log::Info(" CameraSystem not Update %s ", ID.GetString().c_str() );
                }
            }
        }

        virtual void SetNodeType( const std::type_info* Type = &typeid( CameraNode ) )
        {
            m_NodeType = Type;
        }
};

#endif // __SYSTEM_H__