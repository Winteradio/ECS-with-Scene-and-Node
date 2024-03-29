#ifndef __IOBJECT_H__
#define __IOBJECT_H__

#include "MyUUID.h"

class IObject
{
    public :
        IObject() {};
        IObject( MyUUID ID ) : m_ID( ID ) {};
        virtual ~IObject() {};

    public :
        void SetID( MyUUID ID ) { m_ID = ID; }
        MyUUID GetID() { return m_ID; }
        MyUUID GetID() const { return m_ID; }

    protected :
        MyUUID m_ID;
};

#endif // __IOBJECT_H__