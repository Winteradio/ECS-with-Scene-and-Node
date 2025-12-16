#include "Container/EntityContainer.h"

namespace ECS
{
	EntityContainer::EntityContainer()
		: m_storage()
	{}

	EntityContainer::EntityContainer(const EntityContainer& other)
		: m_storage(other.m_storage)
	{}

	EntityContainer::EntityContainer(EntityContainer&& other)
		: m_storage(std::move(other.m_storage))
	{}

	EntityContainer& EntityContainer::operator=(const EntityContainer& other)
	{
		if (this != &other)
		{
			m_storage = other.m_storage;
		}

		return *this;
	}

	EntityContainer& EntityContainer::operator=(EntityContainer&& other)
	{
		if (this != &other)
		{
			m_storage = std::move(other.m_storage);
		}

		return *this;
	}

	bool EntityContainer::operator==(const EntityContainer& other)
	{
		return Object::operator==(other);
	}

	bool EntityContainer::operator!=(const EntityContainer& other)
	{
		return !(*this == other);
	}

	void EntityContainer::Init()
	{
		m_storage.Clear();
	}

	void EntityContainer::Clear()
	{
		m_storage.Clear();
	}

	void EntityContainer::Erase(const UUID& uuid)
	{
		m_storage.Erase(uuid);
	}

	Memory::ObjectPtr<Entity> EntityContainer::Emplace(const std::string& name)
	{
		Memory::ObjectPtr<Entity> entity = Memory::MakePtr<Entity>(name);
		if (entity)
		{
			m_storage[entity->GetID()] = entity;
		}

		return entity;
	}

	Memory::ObjectPtr<Entity> EntityContainer::Get(const UUID& uuid)
	{
		auto itr = m_storage.Find(uuid);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			return Memory::ObjectPtr<Entity>();
		}
	}

	const Memory::ObjectPtr<Entity> EntityContainer::Get(const UUID& uuid) const
	{
		auto itr = m_storage.Find(uuid);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			return Memory::ObjectPtr<Entity>();
		}
	}
}