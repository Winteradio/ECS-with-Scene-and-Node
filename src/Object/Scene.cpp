#include "Object/Scene.h"

namespace ECS
{
	Scene::Scene()
		: m_nodeType()
		, m_nodeID()
		, m_systemID()
		, m_name()
	{}

	Scene::Scene(const std::string& name)
		: m_nodeType()
		, m_nodeID()
		, m_systemID()
		, m_name(name)
	{}

	bool Scene::HasNode(const UUID& nodeId) const
	{
		auto itr = m_nodeID.Find(nodeId);
		
		return itr != m_nodeID.End();
	}

	bool Scene::HasNodeType(const Reflection::TypeInfo* nodeType) const
	{
		const size_t typeHash = nodeType->GetTypeHash();

		auto itr = m_nodeType.Find(typeHash);

		return itr != m_nodeType.End();
	}

	bool Scene::HasSystem(const UUID& systemId) const
	{
		auto itr = m_systemID.Find(systemId);

		return itr != m_systemID.End();
	}

	const Scene::TypeData& Scene::GetNodeType() const
	{
		return m_nodeType;
	}

	const Scene::IDSet& Scene::GetNodeID() const
	{
		return m_nodeID;
	}

	const Scene::IDArray& Scene::GetSystemID() const
	{
		return m_systemID;
	}

	const std::string& Scene::GetName() const
	{
		return m_name;
	}

	void Scene::UpdateSystemID(const IDArray& systemID)
	{
		m_systemID = systemID;
	}

	void Scene::SetName(const std::string& name)
	{
		m_name = name;
	}
}