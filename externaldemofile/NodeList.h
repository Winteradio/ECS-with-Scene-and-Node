#ifndef __NODE_H__
#define __NODE_H__

#include <ECSProject/INode.h>
#include "ComponentList.h"

class RenderNode : public INode
{
    public :
        RenderNode() {};
        RenderNode( MyUUID ID ) : INode( ID ) {};
        virtual ~RenderNode(){};

    public :
        virtual bool Check( MemoryPtr<Entity>& Object )
        {
            if ( !Object->HasComponent<MeshComponent>() ) return false;
            if ( !Object->HasComponent<TransformComponent>() ) return false;

            return true;
        }

        virtual void Init( MemoryPtr<Entity>& Object )
        {
            if ( !Check( Object ) ) return;

            Mesh = ComponentManager::GetHandle().GetComponent<MeshComponent>( Object->GetComponent<MeshComponent>() );
            Trans = ComponentManager::GetHandle().GetComponent<TransformComponent>( Object->GetComponent<TransformComponent>() );
        }

    public :
        MemoryPtr<MeshComponent> Mesh;
        MemoryPtr<TransformComponent> Trans;
};

class CameraNode : public INode
{
    public :
        CameraNode() {};
        CameraNode( MyUUID ID ) : INode( ID ) {};
        virtual ~CameraNode(){};

    public :
        virtual bool Check( MemoryPtr<Entity>& Object )
        {
            return true;
        }

        virtual void Init( MemoryPtr<Entity>& Object )
        {
        }

    public :
};

class CollisionNode : public INode
{
    public :
        CollisionNode() {};
        CollisionNode( MyUUID ID ) : INode( ID ) {};
        virtual ~CollisionNode(){};

    public :
        virtual bool Check( MemoryPtr<Entity>& Object )
        {
            return true;
        }

        virtual void Init( MemoryPtr<Entity>& Object )
        {
        }
};

class PhysicsNode : public INode
{
    public :
        PhysicsNode() {};
        PhysicsNode( MyUUID ID ) : INode( ID ) {};
        virtual ~PhysicsNode(){};

    public :
        virtual bool Check( MemoryPtr<Entity>& Object )
        {
            return true;
        }

        virtual void Init( MemoryPtr<Entity>& Object )
        {
        }
};

#endif // __NODE_H__