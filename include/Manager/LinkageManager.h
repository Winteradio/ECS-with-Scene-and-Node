#ifndef __LINKAGEMANAGER_H__
#define __LINKAGEMANAGER_H__

class LinkageManager
{
    private :
        LinkageManager();
        ~LinkageManager();

    public :
        static LinkageManager& GetHandle();

        void Init();
        void Destroy();
        void Clear();

    private :
        static LinkageManager m_LinkageManager;
};

#endif // __LINKAGEMANAGER_H__