#ifndef __INODE_H__
#define __INODE_H__

#include <MemoryProject/MemoryPtr.h>

#include "IObject.h"
#include "Entity.h"

class INode : public IObject
{
    public :
        INode() {};
        INode( MyUUID ID ) : IObject( ID ) {}
        virtual ~INode(){};

    public :
        virtual bool Check( MemoryPtr<Entity>& EntityMPtr ) = 0;
        virtual void Init( MemoryPtr<Entity>& EntityMPtr ) = 0;
};

#endif // __INODE_H__