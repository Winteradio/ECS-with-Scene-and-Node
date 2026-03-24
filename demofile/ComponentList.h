#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Object/Data.h"

struct ResourceComponent : ECS::Component
{
    using ECS::Component::Component;

    std::string resourceID;

    ResourceComponent(const ECS::UUID& uuid, const std::string& resourceID)
        : ECS::Component(uuid)
        , resourceID(resourceID)
    {};
};

struct MeshComponent : ECS::Component
{
    using ECS::Component::Component;

    float boundingBox[6];
};

struct VelocityComponent : ECS::Component
{
    using ECS::Component::Component;

    float velocity[3];
};

struct TransformComponent : ECS::Component
{
    using ECS::Component::Component;

    float Position[3];
    float Rotation[3];
    float Scale[3];
};

struct ColorComponent : ECS::Component
{
    using ECS::Component::Component;

    float Red;
    float Green;
    float Blue;
    float Alpha;

    ColorComponent(const ECS::UUID & uuid, const float red, const float green, const float blue, const float alpha)
		: ECS::Component(uuid)
        , Red(red)
        , Green(green)
        , Blue(blue)
        , Alpha(alpha)
    {}
};

struct CameraComponent : ECS::Component
{
    using ECS::Component::Component;

    float fov;
    float nearPlane;
    float farPlane;

    float width;
    float height;

    CameraComponent(const ECS::UUID& uuid
        , const float fov
        , const float nearPlane
        , const float farPlane
        , const float width
        , const float height)
		: ECS::Component(uuid)
        , fov(fov)
        , nearPlane(nearPlane)
        , farPlane(farPlane)
        , width(width)
        , height(height)
    {}
};

#endif // __COMPONENT_H__