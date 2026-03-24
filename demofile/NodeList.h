#ifndef __NODE_H__
#define __NODE_H__

#include "ComponentList.h"
#include <Memory/include/Memory.h>

struct RenderNode : ECS::Node
{
    using ECS::Node::Node;

    Memory::ObjectPtr<ResourceComponent> resource;
    Memory::ObjectPtr<TransformComponent> transform;
    Memory::ObjectPtr<ColorComponent> color;

    RenderNode(const ECS::UUID& uuid, const Memory::ObjectPtr<ResourceComponent> resource
        , const Memory::ObjectPtr<TransformComponent> transform
        , const Memory::ObjectPtr<ColorComponent> color)
        : ECS::Node(uuid)
        , resource(resource)
        , transform(transform)
        , color(color)
    {}
};

struct CameraNode : ECS::Node
{
    using ECS::Node::Node;

    Memory::ObjectPtr<CameraComponent> camera;
    Memory::ObjectPtr<TransformComponent> transform;
};

struct CollisionNode : ECS::Node
{
    using ECS::Node::Node;

    Memory::ObjectPtr<MeshComponent> mesh;
    Memory::ObjectPtr<TransformComponent> transform;
};

struct PhysicsNode : ECS::Node
{
    using ECS::Node::Node;

    Memory::ObjectPtr<VelocityComponent> velocity;
    Memory::ObjectPtr<TransformComponent> transform;
};

#endif // __NODE_H__