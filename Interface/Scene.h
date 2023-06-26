#ifndef __SCENE_H__
#define __SCENE_H__

#include "Entity.h"
#include "ISystem.h"

#include <LogProject/Log.h>

class Scene
{
    using EntityID = MyUUIDSet;
    using SystemID = MyUUIDSet;
    using SystemSequence = std::queue< ISystem* >;
    using DoneSequence = std::queue< ISystem* >;

    public :
        Scene();
        Scene( std::string Name );
        Scene( int Index ); 
        Scene( int Index, std::string Name );
        virtual ~Scene();

    public :
        void Init();
        void Destroy();
        void Update( float DeltaTime );

    public :
        void RegisterEntity( MyUUID ID );
        void RemoveEntity( MyUUID ID );
        bool HasEntity( MyUUID ID );

        void RegisterSystem( MyUUID ID );
        void RemoveSystem( MyUUID ID );
        bool HasSystem( MyUUID ID );

    public :
        EntityID& GetEntityIDs();
        SystemID& GetSystemIDs();

        std::string GetName();
        int GetIndex();

        void SetName( std::string Name );
        void SetIndex( int Index );

    private :
        std::string m_Name;
        int m_Index;

        EntityID m_EntityID;
        SystemID m_SystemID;
        SystemSequence m_Sequence;
        DoneSequence m_Done;
};

#endif // __SCENE_H__