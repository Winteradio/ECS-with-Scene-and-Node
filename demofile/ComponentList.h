#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Object/Data.h"

struct ResourceComponent : ECS::Component::Base
{
    std::string resourceID;

    ResourceComponent(const std::string& resourceID)
        : resourceID(resourceID)
    {};
};

struct MeshComponent : ECS::Component::Base
{
    float boundingBox[6];
};

struct VelocityComponent : ECS::Component::Base
{
    float velocity[3];
};

struct TransformComponent : ECS::Component::Base
{
    float Position[3];
    float Rotation[3];
    float Scale[3];
};

struct ColorComponent : ECS::Component::Base
{
    float Red;
    float Green;
    float Blue;
    float Alpha;

    ColorComponent(const float red, const float green, const float blue, const float alpha)
        : Red(red)
        , Green(green)
        , Blue(blue)
        , Alpha(alpha)
    {}
};

struct CameraComponent : ECS::Component::Base
{
    float fov;
    float nearPlane;
    float farPlane;

    float width;
    float height;

    CameraComponent(const float fov
        , const float nearPlane
        , const float farPlane
        , const float width
        , const float height)
        : fov(fov)
        , nearPlane(nearPlane)
        , farPlane(farPlane)
        , width(width)
        , height(height)
    {}
};

#endif // __COMPONENT_H__