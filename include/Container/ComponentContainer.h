#ifndef __ECS_COMPONENT_CONTAINER_H__
#define __ECS_COMPONENT_CONTAINER_H__

#include <Container/include/HashMap.h>
#include <Memory/include/Memory.h>
#include <Reflection/include/Property/PropertyMacro.h>

#include "Container/BaseContainer.h"
#include "Utils.h"

namespace ECS
{
	template<typename T>
	class ComponentContainer : public BaseContainer
	{
		static_assert(Utils::IsComponent<T> && "Invalid Component Type");

		GENERATE(ComponentContainer);

		public :
			using Type = T;
			using Storage = wtr::HashMap<UUID, Memory::ObjectPtr<T>>;

			ComponentContainer()
				: m_storage()
			{}

			ComponentContainer(const ComponentContainer& other)
				: m_storage(other.m_storage)
			{}

			ComponentContainer(ComponentContainer&& other)
				: m_storage(std::move(other.m_storage))
			{}

			ComponentContainer& operator=(const ComponentContainer& other)
			{
				if (this != &other)
				{
					m_storage = other.m_storage;
				}

				return *this;
			}

			ComponentContainer& operator=(ComponentContainer&& other)
			{
				if (this != &other)
				{
					m_storage = std::move(other.m_storage);
				}

				return *this;
			}

			virtual ~ComponentContainer() = default;

			bool operator==(const ComponentContainer& other)
			{
				return Object::operator=(other);
			}

			bool operator!=(const ComponentContainer& other)
			{
				return !(*this == other);
			}

		public :
			void Init() override
			{
				m_storage.Clear();
			}

			void Clear() override
			{
				m_storage.Clear();
			}

			void Erase(const UUID& entityID) override
			{
				m_storage.Erase(entityID);
			}

			template<typename... Args>
			Memory::ObjectPtr<T> Emplace(const UUID& entityID,  Args&&... args)
			{
				auto [itr, inserted] = m_storage.TryEmplace(entityID);
				if (inserted)
				{
					auto& object = itr->second;
					object = Memory::MakePtr<T>(std::forward<Args>(args)...);
				}

				return itr->second;
			}

		public :
			Memory::ObjectPtr<T> Get(const UUID& entityID)
			{
				auto itr = m_storage.Find(entityID);
				if (itr != m_storage.End())
				{
					return itr->second;
				}
				else
				{
					return Memory::ObjectPtr<T>();
				}
			}

			const Memory::ObjectPtr<T> Get(const UUID& entityID) const
			{
				auto itr = m_storage.Find(entityID);
				if (itr != m_storage.End())
				{
					return itr->second;
				}
				else
				{
					return Memory::ObjectPtr<T>();
				}
			}

		private :
			PROPERTY(m_storage);
			Storage m_storage;
	};
};

#endif // __ECS_COMPONENT_CONTAINER_H__