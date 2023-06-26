#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <ECSProject/IComponent.h>

class TransformComponent : public IComponent
{
    public :
        TransformComponent() {};
        TransformComponent( MyUUID ID ) : IComponent( ID ) {}
        virtual ~TransformComponent() {};
};

#endif // __COMPONENT_H__