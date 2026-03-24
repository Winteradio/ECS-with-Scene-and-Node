#include "Object/Entity.h"

namespace ECS
{
	Entity::Entity()
		: Object()
		, m_componentType()
		, m_nodeType()
		, m_name()
	{}

	Entity::Entity(const std::string& name)
		: Object()
		, m_componentType()
		, m_nodeType()
		, m_name(name)
	{}

	Entity::Entity(const Entity& other)
		: Object(other.m_uuid)
		, m_componentType(other.m_componentType)
		, m_nodeType(other.m_nodeType)
		, m_name(other.m_name)
	{}

	Entity::Entity(Entity&& other)
		: Object(std::move(other))
		, m_componentType(std::move(other.m_componentType))
		, m_nodeType(std::move(other.m_nodeType))
		, m_name(std::move(other.m_name))
	{}

	Entity& Entity::operator=(const Entity& other)
	{
		if (this != &other)
		{
			Object::operator=(other);

			m_componentType = other.m_componentType;
			m_nodeType = other.m_nodeType;
			m_name = other.m_name;
		}

		return *this;
	}

	Entity& Entity::operator=(Entity&& other)
	{
		if (this != &other)
		{
			Object::operator=(other);

			m_componentType = std::move(other.m_componentType);
			m_nodeType = std::move(other.m_nodeType);
			m_name = std::move(m_name);
		}

		return *this;
	}

	bool Entity::operator==(const Entity& other) const
	{
		return Object::operator==(other);
	}

	bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	void Entity::AddComponent(const Reflection::TypeInfo* componentType)
	{
		m_componentType.Insert(componentType);
	}

	void Entity::RemoveComponent(const Reflection::TypeInfo* componentType)
	{
		m_componentType.Erase(componentType);
	}

	bool Entity::HasComponent(const Reflection::TypeInfo* componentType) const
	{
		auto itr = m_componentType.Find(componentType);
		return itr != m_componentType.End();
	}

	void Entity::AddNode(const Reflection::TypeInfo* nodeType)
	{
		m_nodeType.Insert(nodeType);
	}

	void Entity::RemoveNode(const Reflection::TypeInfo* nodeType)
	{
		m_nodeType.Erase(nodeType);
	}

	bool Entity::HasNode(const Reflection::TypeInfo* nodeType) const
	{
		auto itr = m_nodeType.Find(nodeType);
		return itr != m_nodeType.End();
	}

	const Entity::TypeData& Entity::GetComponentType() const
	{
		return m_componentType;
	}

	const Entity::TypeData& Entity::GetNodeType() const
	{
		return m_nodeType;
	}

	const std::string& Entity::GetName() const
	{
		return m_name;
	}
}