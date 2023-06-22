#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>
#include "INode.h"

class NodeManager
{
    private :
        NodeManager();
        ~NodeManager();

    public :
        static NodeManager& GetHandle();

        void Init();
        void Destroy();

    private :
        static NodeManager m_NodeManager;
};

#endif // __NODEMANAGER_H__