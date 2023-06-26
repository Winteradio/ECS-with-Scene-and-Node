#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <ECSProject/ISystem.h>

class RenderSystem : public ISystem
{
    public :
        RenderSystem() {};
        RenderSystem( MyUUID ID ) : ISystem( ID ) {};
        virtual ~RenderSystem() {};

    public :
        virtual void Update( float Deltatime )
        {
            Log::Info(" RenderSystem Update ");
        }
};

class PhysicsSystem : public ISystem
{
    public :
        PhysicsSystem() {};
        PhysicsSystem( MyUUID ID ) : ISystem( ID ) {};
        virtual ~PhysicsSystem() {};

    public :
        virtual void Update( float Deltatime )
        {
            Log::Info(" PhysicsSystem Update ");
        }
};

class MoveSystem : public ISystem
{
    public :
        MoveSystem() {};
        MoveSystem( MyUUID ID ) : ISystem( ID ) {};
        virtual ~MoveSystem() {};

    public :
        virtual void Update( float Deltatime )
        {
            Log::Info(" MoveSystem Update ");
        }
};

class CollisionSystem : public ISystem
{
    public :
        CollisionSystem() {};
        CollisionSystem( MyUUID ID ) : ISystem( ID ) {};
        virtual ~CollisionSystem() {};

    public :
        virtual void Update( float Deltatime )
        {
            Log::Info(" CollisionSystem Update ");
        }
};

class CameraSystem : public ISystem
{
    public :
        CameraSystem() {};
        CameraSystem( MyUUID ID ) : ISystem( ID ) {};
        virtual ~CameraSystem() {};

    public :
        virtual void Update( float Deltatime )
        {
            Log::Info(" CameraSystem Update ");
        }
};

#endif // __SYSTEM_H__