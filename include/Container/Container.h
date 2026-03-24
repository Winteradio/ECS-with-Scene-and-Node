#ifndef __ECS_CONTAINER_H__
#define __ECS_CONTAINER_H__

#include <Reflection/include/Macro.h>
#include <Reflection/include/Utils.h>
#include <Container/include/HashMap.h>
#include <Memory/include/Pointer/ObjectPtr.h>

#include "Object/Object.h"

namespace ECS
{
	template<typename Base = Object>
	class BaseContainer
	{
		GENERATE(BaseContainer);

	public :
		using BaseType = Base;

		BaseContainer() = default;
		virtual ~BaseContainer() = default;

		virtual void Clear() = 0;
		virtual void Erase(const UUID& uuid) = 0;

		virtual Memory::ObjectPtr<Base> GetBase(const UUID& uuid) = 0;
		virtual Memory::ObjectPtr<const Base> GetBase(const UUID& uuid) const = 0;
	};

	template<typename T, typename Base = Object>
	class Container : public BaseContainer<Base>
	{
		GENERATE(Container);

	public:
		using Type = T;
		using Storage = wtr::HashMap<UUID, Memory::ObjectPtr<T>>;

		Container() = default;
		virtual ~Container() = default;

		bool operator==(const Container& other)
		{
			const Reflection::TypeInfo* thisType = this->GetTypeInfo();
			const Reflection::TypeInfo* otherType = other.GetTypeInfo();

			if (Reflection::IsSame(thisType, otherType))
			{
				return m_storage == other.m_storage;
			}
			else
			{
				return false;
			}
		}

		bool operator!=(const Container& other)
		{
			return !(*this == other);
		}

	public:
		void Clear() override
		{
			m_storage.Clear();
		}

		void Erase(const UUID& uuid) override
		{
			m_storage.Erase(uuid);
		}

		Memory::ObjectPtr<T> Get(const UUID& uuid)
		{
			auto itr = m_storage.Find(uuid);
			if (itr != m_storage.End())
			{
				return itr->second;
			}
			else
			{
				return Memory::ObjectPtr<T>();
			}
		}

		Memory::ObjectPtr<const T> Get(const UUID& uuid) const
		{
			auto itr = m_storage.Find(uuid);
			if (itr != m_storage.End())
			{
				return itr->second;
			}
			else
			{
				return Memory::ObjectPtr<const T>();
			}
		}

		Memory::ObjectPtr<Base> GetBase(const UUID& uuid) override
		{
			return Get(uuid);
		}

		Memory::ObjectPtr<const Base> GetBase(const UUID& uuid) const override
		{
			return Get(uuid);
		}

		Storage& GetStorage()
		{
			return m_storage;
		}

	public :
		template<typename... Args>
		Memory::ObjectPtr<T> Emplace(Args&&... args)
		{
			Memory::ObjectPtr<T> object = Memory::MakePtr<T>(std::forward<Args>(args)...);
			if (object)
			{
				m_storage[object->GetID()] = object;
			}

			return object;
		}

	private :
		PROPERTY(m_storage);
		Storage m_storage;
	};
};

#endif // __ECS_CONTAINER_H__