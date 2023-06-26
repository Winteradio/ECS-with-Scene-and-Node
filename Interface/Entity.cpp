#include "Entity.h"

Entity::Entity() {}
Entity::Entity( std::string Name ) : m_Name( Name ) {}
Entity::Entity( MyUUID ID ) : IObject( ID ) {}
Entity::Entity( MyUUID ID, std::string Name ) : IObject( ID ), m_Name( Name ) {}
Entity::~Entity() {}

void Entity::SetName( std::string Name ) { m_Name = Name; }

std::string& Entity::GetName() { return m_Name; }
Entity::Data& Entity::GetData() { return m_Data; }