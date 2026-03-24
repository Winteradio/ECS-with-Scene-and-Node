#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include <Container/Container.h>

namespace ECS
{
	template<typename Base = Object>
	class Registry
	{
		GENERATE(Registry);

	public :
		using BaseType = Base;
		using BaseContainerType = BaseContainer<BaseType>;
		using ContainerData = wtr::HashMap<size_t, Memory::ObjectPtr<BaseContainerType>>;

		template<typename T>
		using ContainerType = Container<T, BaseType>;

		Registry() = default;
		~Registry() = default;

	public :
		void Clear()
		{
			m_containerData.Clear();
		}

		void Remove(const UUID& uuid, const Reflection::TypeInfo* typeInfo)
		{
			Memory::ObjectPtr<BaseContainerType> container = GetContainer(typeInfo);
			if (container)
			{
				container->Erase(uuid);
			}
		}

		Memory::ObjectPtr<BaseContainerType> GetContainer(const Reflection::TypeInfo* typeInfo)
		{
			const size_t typeHash = typeInfo->GetTypeHash();

			auto itr = m_containerData.Find(typeHash);
			if (itr != m_containerData.End())
			{
				return itr->second;
			}

			return Memory::ObjectPtr<BaseContainerType>();
		}

	public :
		template<typename T, typename... Args>
		Memory::ObjectPtr<T> Create(Args&&... args)
		{
			Memory::ObjectPtr<ContainerType<T>> container = GetContainer<T>();
			if (!container)
			{
				container = CreateContainer<T>();

				const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
				const size_t typeHash = typeInfo->GetTypeHash();

				m_containerData[typeHash] = container;
			}

			return container->Emplace(std::forward<Args>(args)...);
		}

		template<typename T>
		void Remove(const UUID& uuid)
		{
			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Remove(uuid, typeInfo);
		}


		template<typename T>
		Memory::ObjectPtr<ContainerType<T>> GetContainer()
		{
			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			const size_t typeHash = typeInfo->GetTypeHash();

			Memory::ObjectPtr<BaseContainerType> baseContainer = GetContainer(typeInfo);
			if (baseContainer)
			{
				Memory::ObjectPtr<ContainerType<T>> container = Memory::Cast<ContainerType<T>>(baseContainer);
				return container;
			}

			return Memory::ObjectPtr<ContainerType<T>>();
		}

	private:
		template<typename T>
		Memory::ObjectPtr<ContainerType<T>> CreateContainer()
		{
			Memory::ObjectPtr<ContainerType<T>> container = Memory::MakePtr<ContainerType<T>>();

			return container;
		}

	private:
		PROPERTY(m_containerData);
		ContainerData m_containerData;
	};
};

#endif // __ECS_REGISTRY_H__