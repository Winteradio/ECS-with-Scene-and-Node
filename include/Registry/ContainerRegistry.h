#ifndef __ECS_CONTAINER_REGISTRY_H__
#define __ECS_CONTAINER_REGISTRY_H__

#include <Container/include/HashMap.h>
#include <Memory/include/Memory.h>
#include <Reflection/include/Type/TypeMacro.h>
#include <Reflection/include/Property/PropertyMacro.h>

namespace ECS
{
	template<template<typename> typename Container>
	class ContainerRegistry
	{
		GENERATE(ContainerRegistry);

		public :
			using ContainerData = wtr::HashMap<size_t, Memory::ObjectPtr<BaseContainer>>;

			ContainerRegistry() = default;
			~ContainerRegistry() = default;

		public :
			template<typename T, typename... Args>
			Memory::ObjectPtr<T> Create(const UUID& entityID, Args&&... args)
			{
				Memory::ObjectPtr<Container<T>> container = GetContainer<T>();
				if (container)
				{
					return container->Emplace(entityID, std::forward<Args>(args)...);
				}

				return Memory::ObjectPtr<T>();
			}

			template<typename T>
			void Remove(const Memory::ObjectPtr<T>& node)
			{
				if (node)
				{
					const UUID& entityID = node->GetID();
					const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
					Remove(entityID, typeInfo);
				}
			}

			template<typename T>
			void Remove(const UUID& entityID)
			{
				const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
				Remove(entityID, typeInfo);
			}

			template<typename T>
			Memory::ObjectPtr<Container<T>> GetContainer()
			{
				const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
				const size_t typeHash = typeInfo->GetTypeHash();

				auto [itr, inserted] = m_containerData.TryEmplace(typeHash);
				if (inserted)
				{
					auto& container = itr->second;
					container = CreateContainer<T>();
				}

				Memory::ObjectPtr<Container<T>> container = Memory::Cast<Container<T>>(itr->second);

				return container;
			}

		public :
			void Clear()
			{
				m_containerData.Clear();
			}

			void Remove(const UUID& entityID, const Reflection::TypeInfo* typeInfo)
			{
				Memory::ObjectPtr<BaseContainer> container = GetContainer(typeInfo);
				if (container)
				{
					container->Erase(entityID);
				}
			}

			Memory::ObjectPtr<BaseContainer> GetContainer(const Reflection::TypeInfo* typeInfo)
			{
				const size_t typeHash = typeInfo->GetTypeHash();
				auto itr = m_containerData.Find(typeHash);
				if (itr != m_containerData.End())
				{
					return itr->second;
				}

				return Memory::ObjectPtr<BaseContainer>();
			}

		private :
			template<typename T>
			Memory::ObjectPtr<Container<T>> CreateContainer()
			{
				Memory::ObjectPtr<Container<T>> container = Memory::MakePtr<Container<T>>();

				return container;
			}

		private :
			PROPERTY(m_containerData);
			ContainerData m_containerData;
	};
};

#endif // __ECS_CONTAINER_REGISTRY_H__