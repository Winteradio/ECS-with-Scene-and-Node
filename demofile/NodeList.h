#ifndef __NODE_H__
#define __NODE_H__

#include "ComponentList.h"
#include <Memory/include/Memory.h>

struct RenderNode : ECS::Node::Base
{
    Memory::ObjectPtr<ResourceComponent> resource;
    Memory::ObjectPtr<TransformComponent> transform;
    Memory::ObjectPtr<ColorComponent> color;

    RenderNode(const Memory::ObjectPtr<ResourceComponent> resource
        , const Memory::ObjectPtr<TransformComponent> transform
        , const Memory::ObjectPtr<ColorComponent> color)
        : resource(resource)
        , transform(transform)
        , color(color)
    {}
};

struct CameraNode : ECS::Node::Base
{
    Memory::ObjectPtr<CameraComponent> camera;
    Memory::ObjectPtr<TransformComponent> transform;
};

struct CollisionNode : ECS::Node::Base
{
    Memory::ObjectPtr<MeshComponent> mesh;
    Memory::ObjectPtr<TransformComponent> transform;
};

struct PhysicsNode : ECS::Node::Base
{
    Memory::ObjectPtr<VelocityComponent> velocity;
    Memory::ObjectPtr<TransformComponent> transform;
};

#endif // __NODE_H__