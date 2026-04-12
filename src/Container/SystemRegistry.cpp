#include "Container/SystemRegistry.h"

namespace ECS
{
	void SystemRegistry::Clear()
	{
		m_storage.Clear();
		m_typeData.Clear();
	}

	void SystemRegistry::Remove(const Reflection::TypeInfo* typeInfo)
	{
		const size_t typeHash = typeInfo->GetTypeHash();

		auto& systemID = m_typeData[typeHash];

		m_storage.Erase(systemID);
		m_typeData.Erase(typeHash);
		m_graph.Remove(systemID);
	}

	void SystemRegistry::Remove(const UUID& systemID)
	{
		auto itr = m_storage.Find(systemID);
		if (itr == m_storage.End())
		{
			return;
		}

		auto& system = itr->second;
		const Reflection::TypeInfo* typeInfo = system->GetTypeInfo();
		const size_t typeHash = typeInfo->GetTypeHash();

		m_storage.Erase(itr);
		m_typeData.Erase(typeHash);
		m_graph.Remove(systemID);
	}

	Memory::ObjectPtr<System> SystemRegistry::Get(const Reflection::TypeInfo* typeInfo)
	{
		const size_t typeHash = typeInfo->GetTypeHash();

		auto typeITR = m_typeData.Find(typeHash);
		if (typeITR == m_typeData.End())
		{
			return Memory::ObjectPtr<System>();
		}

		auto& systemID = typeITR->second;

		auto systemITR = m_storage.Find(systemID);
		if (systemITR == m_storage.End())
		{
			return Memory::ObjectPtr<System>();
		}

		return systemITR->second;
	}

	Memory::ObjectPtr<System> SystemRegistry::Get(const UUID& systemID)
	{
		auto itr = m_storage.Find(systemID);
		if (itr != m_storage.End())
		{
			return itr->second;
		}

		return Memory::ObjectPtr<System>();
	}

	const Memory::ObjectPtr<System> SystemRegistry::Get(const Reflection::TypeInfo* typeInfo) const
	{
		const size_t typeHash = typeInfo->GetTypeHash();

		auto typeITR = m_typeData.Find(typeHash);
		if (typeITR == m_typeData.End())
		{
			return Memory::ObjectPtr<System>();
		}

		auto& systemID = typeITR->second;

		auto systemITR = m_storage.Find(systemID);
		if (systemITR == m_storage.End())
		{
			return Memory::ObjectPtr<System>();
		}

		return systemITR->second;
	}

	const Memory::ObjectPtr<System> SystemRegistry::Get(const UUID& systemID) const
	{
		auto itr = m_storage.Find(systemID);
		if (itr != m_storage.End())
		{
			return itr->second;
		}

		return Memory::ObjectPtr<System>();
	}

	const SystemRegistry::GraphType SystemRegistry::BuildGraph()
	{
		for (const auto& systemPair : m_storage)
		{
			const auto& systemID = systemPair.first;
			const auto& system = systemPair.second;

			if (!system)
			{
				continue;
			}

			const auto& dependData = system->GetDependData();

			for (const auto& dependID : dependData)
			{
				m_graph.Add(systemID, dependID);
			}
		}

		if (m_graph.IsUpdated())
		{
			m_graph.Build();
		}

		return m_graph;
	}
}
