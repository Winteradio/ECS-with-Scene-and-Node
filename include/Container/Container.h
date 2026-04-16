#ifndef __ECS_CONTAINER_H__
#define __ECS_CONTAINER_H__

#include <Reflection/include/Utils.h>

#include "Container/Storage.h"
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

		bool operator==(const BaseContainer& other)
		{
			const Reflection::TypeInfo* thisType = this->GetTypeInfo();
			const Reflection::TypeInfo* otherType = other.GetTypeInfo();
			if (Reflection::IsSame(thisType, otherType))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator!=(const BaseContainer& other)
		{
			return !(*this == other);
		}

	public :
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
		using StorageType = DataStorage<T>;

		Container() = default;
		virtual ~Container() = default;

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
			return m_storage.Get(uuid);
		}

		Memory::ObjectPtr<const T> Get(const UUID& uuid) const
		{
			return m_storage.Get(uuid);
		}

		Memory::ObjectPtr<Base> GetBase(const UUID& uuid) override
		{
			return m_storage.Get(uuid);
		}

		Memory::ObjectPtr<const Base> GetBase(const UUID& uuid) const override
		{
			return m_storage.Get(uuid);
		}

		StorageType& GetStorage()
		{
			return m_storage;
		}

	public :
		template<typename... Args>
		Memory::ObjectPtr<T> Emplace(const UUID& id, Args&&... args)
		{
			return m_storage.Emplace(id, std::forward<Args>(args)...);
		}

	private :
		PROPERTY(m_storage);
		StorageType m_storage;
	};
};

#endif // __ECS_CONTAINER_H__