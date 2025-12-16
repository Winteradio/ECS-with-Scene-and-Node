#include "Object/Entity.h"

namespace ECS
{
	Entity::Entity()
		: Object()
		, m_componentType()
		, m_name()
	{}

	Entity::Entity(const std::string& name)
		: Object()
		, m_componentType()
		, m_name(name)
	{}

	Entity::Entity(const Entity& other)
		: Object(other.m_uuid)
		, m_componentType(other.m_componentType)
		, m_name(other.m_name)
	{}

	Entity::Entity(Entity&& other)
		: Object(std::move(other))
		, m_componentType(std::move(other.m_componentType))
		, m_name(std::move(other.m_name))
	{}

	Entity& Entity::operator=(const Entity& other)
	{
		if (this != &other)
		{
			Object::operator=(other);

			m_componentType = other.m_componentType;
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
		const size_t typeHash = componentType->GetTypeHash();
		m_componentType.Insert(typeHash);
	}

	void Entity::RemoveComponent(const Reflection::TypeInfo* componentType)
	{
		const size_t typeHash = componentType->GetTypeHash();

		m_componentType.Erase(typeHash);
	}

	bool Entity::HasComponent(const Reflection::TypeInfo* componentType) const
	{
		const size_t typeHash = componentType->GetTypeHash();

		auto itr = m_componentType.Find(typeHash);
		return itr != m_componentType.End();
	}

	const Entity::TypeData& Entity::GetTypeData() const
	{
		return m_componentType;
	}

	const std::string& Entity::GetName() const
	{
		return m_name;
	}
}