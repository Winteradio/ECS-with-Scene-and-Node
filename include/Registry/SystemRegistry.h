#ifndef __ECS_SYSTEM_REGISTRY_H__
#define __ECS_SYSTEM_REGISTRY_H__

#include <Container/include/HashMap.h>
#include <Memory/include/Memory.h>
#include <Reflection/include/Type/TypeMacro.h>
#include <Reflection/include/Property/PropertyMacro.h>

#include "UUID/UUID.h"
#include "Graph/Graph.h"
#include "Object/System.h"

namespace ECS
{
	class SystemRegistry
	{
		GENERATE(SystemRegistry);

	private:
		struct UUIDString
		{
			std::string operator()(const UUID& uuid) const
			{
				return uuid.ToString();
			}
		};

	public :
		using Storage = wtr::HashMap<UUID, Memory::ObjectPtr<System>>;
		using TypeData = wtr::HashMap<size_t, UUID>;
		using GraphType = Graph<UUID, UUIDString>;

		SystemRegistry() = default;
		~SystemRegistry() = default;

	public :
		template<typename T>
		Memory::ObjectPtr<T> Create()
		{
			static_assert(Utils::IsSystem<T> && "Invalid the system type");

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			const size_t typeHash = typeInfo->GetTypeHash();

			auto [itr, inserted] = m_typeData.TryEmplace(typeHash);
			if (inserted)
			{
				Memory::ObjectPtr<T> system = Memory::MakePtr<T>();

				auto& systemID = itr->second;
				systemID = system->GetID();

				auto& baseSystem = m_storage[system->GetID()];
				baseSystem = system;

				return system;
			}
			else
			{
				return Get<T>();
			}
		}

		template<typename T>
		void Remove()
		{
			static_assert(Utils::IsSystem<T> && "Invalid the system type");

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();

			Remove(typeInfo);
		}

		template<typename T>
		Memory::ObjectPtr<T> Get()
		{
			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();

			Memory::ObjectPtr<System> baseSystem = Get(typeInfo);
			Memory::ObjectPtr<T> system = Memory::Cast<T>(baseSystem);

			return system;
		}

		template<typename T>
		const Memory::ObjectPtr<T> Get() const
		{
			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();

			Memory::ObjectPtr<System> baseSystem = Get(typeInfo);
			Memory::ObjectPtr<T> system = Memory::Cast<T>(baseSystem);

			return system;
		}

	public:
		void Clear();
		void Remove(const Reflection::TypeInfo* typeInfo);
		void Remove(const UUID& systemID);

		Memory::ObjectPtr<System> Get(const Reflection::TypeInfo* typeInfo);
		Memory::ObjectPtr<System> Get(const UUID& systemID);

		const Memory::ObjectPtr<System> Get(const Reflection::TypeInfo* typeInfo) const;
		const Memory::ObjectPtr<System> Get(const UUID& systemID) const;

	public :
		const GraphType BuildGraph() const;

	private:
		PROPERTY(m_storage);
		Storage m_storage;

		TypeData m_typeData;
	};
}
#endif // __ECS_SYSTEM_REGISTRY_H__