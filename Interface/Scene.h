#ifndef __SCENE_H__
#define __SCENE_H__

#include "Entity.h"
#include "ISystem.h"

#include <LogProject/Log.h>

class Scene
{
    using TypeMyUUIDUnSetUnMap = std::unordered_map< const std::type_info*, MyUUIDUnSet >;
    using TypeUnSet = std::unordered_set< const std::type_info* >;

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
        template< typename T >
        void RegisterNode( MyUUID ID ) { RegisterNode( &typeid( T ), ID ); }

        template< typename T >
        void RemoveNode( MyUUID ID ) { RemoveNode( &typeid( T ), ID ); }

        template< typename T >
        MyUUIDUnSet& GetNodeIDData() { return GetNodeIDData( &typeid( T ) ); }
        
    private :
        template< typename T >
        bool HasNode( MyUUID ID ) { return HasNode( &typeid( T ), ID ); }

        template< typename T >
        bool HasNodeType() { return HasNodeType( &typeid( T ) ); }

    public :
        void RegisterNode( const std::type_info* Type, MyUUID ID );
        void RemoveNode( const std::type_info* Type, MyUUID ID );
    private :
        bool HasNode( const std::type_info* Type, MyUUID ID );
        bool HasNodeType( const std::type_info* Type );


    public :
        void RegisterSystem( MyUUID ID );
        void RegisterSystem( const std::type_info* Type, MyUUID ID );

        void RemoveSystem( MyUUID ID );
    private :
        bool HasSystem( MyUUID ID );
        

    private :
        void CheckSystemValidity( const std::type_info* Type );
        void ClearSequence( ISystemQueue& Sequence );
        void UpdateSequence();


    public :
        TypeUnSet& GetNodeTypeData();

        MyUUIDUnSet& GetNodeIDData( const std::type_info* Type );
        MyUUIDUnSet& GetSystemIDData();

        std::string GetName();
        int GetIndex();

        void SetName( std::string Name );
        void SetIndex( int Index );

    private :
        TypeUnSet m_NodeTypeData;
        TypeMyUUIDUnSetUnMap m_NodeIDData;
        TypeMyUUIDUnSetUnMap m_PendingSystemIDData;
        MyUUIDUnSet m_SystemIDData;
        ISystemQueue m_MainSequence;
        ISystemQueue m_DoneSequence;

        std::string m_Name;
        int m_Index;
};

#endif // __SCENE_H__