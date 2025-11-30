#ifndef __ILINKAGE_H__
#define __ILINKAGE_H__

#include "MyUUID.h"

class ILinkage
{
    public :
        ILinkage() {};
        virtual ~ILinkage() {};

    public :
        virtual void Connect() = 0;
        virtual void Disconnect() = 0;
        virtual bool IsConnected() = 0;
};

#endif // __ILINKAGE_H__