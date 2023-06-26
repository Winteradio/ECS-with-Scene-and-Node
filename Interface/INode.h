#ifndef __INODE_H__
#define __INODE_H__

#include "IObject.h"
#include "Entity.h"

class INode : public IObject
{
    public :
        INode() {};
        INode( MyUUID ID ) : IObject( ID ) {}
        virtual ~INode(){};

    public :
        virtual bool Init( Entity& Object ) = 0;
};

#endif // __INODE_H__