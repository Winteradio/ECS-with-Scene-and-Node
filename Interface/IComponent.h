#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__

#include "IObject.h"

class IComponent : public IObject
{
    public :
        IComponent() {};
        IComponent( MyUUID ID ) : IObject( ID ) {};
        virtual ~IComponent(){};
};

using MyUUIDIComponentMPtrUnMap = MyUUIDUnMap< MemoryPtr<IComponent> >;

#endif // __ICOMPONENT_H__