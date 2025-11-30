#include "Entity.h"

Entity::Entity() {}
Entity::Entity( std::string Name ) : m_Name( Name ) {}
Entity::Entity( MyUUID ID ) : IObject( ID ) {}
Entity::Entity( MyUUID ID, std::string Name ) : IObject( ID ), m_Name( Name ) {}
Entity::~Entity() {}

void Entity::AddComponent( const std::type_info* Type, MyUUID ID )
{
    bool Check = HasComponent( Type );
    if ( !Check ) m_ComponentIDData[ Type ] = ID;
}

void Entity::RemoveComponent( const std::type_info* Type )
{
    bool Check = HasComponent( Type );
    if ( Check ) m_ComponentIDData.erase( Type );
}

MyUUID Entity::GetComponent( const std::type_info* Type )
{
    bool Check = HasComponent( Type );

    if ( !Check )
    {
        throw Except( " Entity | %s | This Entity has not type of %s component ", __FUNCTION__, Type->name() );
    }

    return m_ComponentIDData[ Type ];
}

bool Entity::HasComponent( const std::type_info* Type )
{
    auto ITR = m_ComponentIDData.find( Type );
    if ( ITR != m_ComponentIDData.end() ) { return true; }
    else { return false; }
}

Entity::TypeUnSet Entity::GetTypeData()
{
    TypeUnSet TypeData;
    for ( auto [ Type, ID ] : m_ComponentIDData )
    {
        TypeData.insert( Type );
    }

    return TypeData;
}

Entity::TypeMyUUIDUnMap& Entity::GetIDData() { return m_ComponentIDData; }
std::string& Entity::GetName() { return m_Name; }

void Entity::SetName( std::string Name ) { m_Name = Name; }