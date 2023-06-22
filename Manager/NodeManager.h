#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include <MemoryProject/MemoryManager.h>
#include "INode.h"

class NodeManager
{
    public :
        NodeManager();
        ~NodeManager();

    public :
        void Init();
        void Destroy();

    private :
};

#endif // __NODEMANAGER_H__