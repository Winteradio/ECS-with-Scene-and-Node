#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMNANAGER_H__

#include <LogProject/Log.h>
#include <MemoryProject/MemoryManager.h>
#include "ISystem.h"

class SystemManager
{
    private :
        SystemManager();
        ~SystemManager();

    public :
        static SystemManager& GetHandle();

        void Init();
        void Destroy();

    private :
        static SystemManager m_SystemManager;
};

#endif // __SYSTEMMANAGER_H__