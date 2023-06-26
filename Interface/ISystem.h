#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__

#include "IObject.h"

class ISystem : public IObject
{
    public :
        ISystem() {};
        ISystem( MyUUID ID ) : IObject( ID ) {}
        virtual ~ISystem(){};

    public :
        virtual void Update( float DeltaTime ) = 0;
};

#endif // __ISYSTEM_H__