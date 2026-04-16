#ifndef __ECS_GRAPH_H__
#define __ECS_GRAPH_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Container/include/DynamicArray.h>
#include <Container/include/HashSet.h>
#include <Container/include/HashMap.h>

#include <Log/include/Log.h>
#include <Object/Object.h>

namespace ECS
{
	class GraphObserver
	{
	public :
		GraphObserver() = default;
		virtual ~GraphObserver() = default;

	public :
		void onInit();
		void onUpdate();
		const bool IsUpdated() const;

	private :
		bool m_updated;
	};

	template<typename T>
	class GraphNode
	{
	public:
		static_assert(Reflection::Utils::IsBase<ECS::Object, T>::value, "The value type must be derived from the graph's base type.");

		using DependencyData = wtr::HashSet<ECS::UUID>;
		using DataType = Memory::RefPtr<T>;

		DataType data;

	public :
		GraphNode()
			: data()
			, m_observer(nullptr)
		{}

		~GraphNode() = default;

	public:
		void SetObserver(GraphObserver* observer)
		{
			m_observer = observer;
		}

		void SetData(const DataType refData)
		{
			data = refData;

			if (m_observer)
			{
				m_observer->onUpdate();
			}
		}

		void SetDependency(const GraphNode& other)
		{
			if (!other.data)
			{
				return;
			}

			m_dependData.Insert(other.data->GetID());

			if (m_observer)
			{
				m_observer->onUpdate();
			}
		}

		void RemoveDependency(const GraphNode& other)
		{
			if (!other.data)
			{
				return;
			}

			m_dependData.Erase(other.data->GetID());

			if (m_observer)
			{
				m_observer->onUpdate();
			}
		}

		const DependencyData& GetDependData() const
		{
			return m_dependData;
		}

	private:
		GraphObserver* m_observer;
		DependencyData m_dependData;
	};

	template<typename Base, typename BaseNode = GraphNode<Base>>
	class Graph : public GraphObserver
	{
	public :
		using NodeData = wtr::HashMap<UUID, BaseNode>;
		using SortedNode = wtr::DynamicArray<typename BaseNode::DataType>;

	public :

	public :
		Graph()
			: GraphObserver()
			, m_nodeData()
			, m_sortedNode()
		{}

		~Graph() = default;

	public:
		template<typename T, typename... Args>
		Memory::RefPtr<T> Create(Args&&... args)
		{
			static_assert(Reflection::Utils::IsBase<Base, T>::value, "The value type must be derived from the graph's base type.");
			
			Memory::RefPtr<T> data = Memory::MakeRef<T>(std::forward<Args>(args)...);
			if (!data)
			{
				LOGERROR() << "[Graph] Failed to create the node data";
				return {};
			}

			GraphNode<Base> node;
			node.SetObserver(this);
			node.SetData(data);

			m_nodeData[data->GetID()] = node;

			return data;
		}

		template<typename T = Base>
		Memory::RefPtr<T> Get(const ECS::UUID& id)
		{
			auto itr = m_nodeData.Find(id);
			if (itr == m_nodeData.End())
			{
				return {};
			}

			auto& node = itr->second;

			return node.data;
		}

		template<typename T = Base>
		Memory::RefPtr<const T> Get(const ECS::UUID& id) const
		{
			auto itr = m_nodeData.Find(id);
			if (itr == m_nodeData.End())
			{
				return {};
			}

			const auto& node = itr->second;

			return node.data;
		}

		void Remove(const ECS::UUID& id)
		{
			auto itr = m_nodeData.Find(id);
			if (itr == m_nodeData.End())
			{
				return;
			}

			for (auto& [id, node] : m_nodeData)
			{
				node.RemoveDependency(itr->second);
			}

			m_nodeData.Erase(itr);
		}

		void Clear()
		{
			m_nodeData.Clear();
			m_sortedNode.Clear();
			onInit();
		}

		const SortedNode& GetSorted()
		{
			if (IsUpdated())
			{
				if (!Build())
				{
					m_sortedNode.Clear();
				}
			}

			onInit();

			return m_sortedNode;
		}

	private :
		bool Build()
		{			
			using IDIndgree = wtr::HashMap<UUID, size_t>;
			using IDData = wtr::HashSet<UUID>;
			using IDEdge = wtr::HashMap<UUID, IDData>;
			using IDQueue = std::queue<UUID>;
			using SortedID = wtr::DynamicArray<UUID>;

			SortedID sortedID;
			IDIndgree idIndegree;
			IDQueue idQueue;
			IDEdge idEdge;

			for (const auto& [id, node] : m_nodeData)
			{
				const auto& dependData = node.GetDependData();
				idIndegree[id] = dependData.Size();

				for (const auto& dependID : dependData)
				{
					idEdge[dependID].Insert(id);
				}
			}

			for (const auto& [id, indegree] : idIndegree)
			{
				if (indegree == 0)
				{
					idQueue.push(id);
				}
			}

			while (!idQueue.empty())
			{
				UUID nodeID = idQueue.front();
				idQueue.pop();

				const auto& dependData = idEdge[nodeID];
				for (const auto& dependID : dependData)
				{
					idIndegree[dependID]--;

					if (idIndegree[dependID] == 0)
					{
						idQueue.push(dependID);
					}
				}

				sortedID.PushBack(nodeID);
			}

			if (sortedID.Size() != idIndegree.Size())
			{
				LOGINFO() << "[GRAPH] Failed to build, cause the circular loop occured";

				for (const auto& [id, node] : m_nodeData)
				{
					auto itr = sortedID.Find(id);
					if (itr != sortedID.End())
					{
						continue;
					}
				
					const auto& dependData = node.GetDependData();
					for (const auto& dependID : dependData)
					{
						LOGINFO() << "[GRAPH] Expected the circular : " << dependID.ToString() << " -> " << id.ToString();
					}
				}

				return false;
			}
			else
			{
				m_sortedNode.Clear();

				for (const auto& id : sortedID)
				{
					auto itr = m_nodeData.Find(id);
					if (itr == m_nodeData.End())
					{
						continue;
					}

					auto& node = itr->second;

					m_sortedNode.PushBack(node.data);
				}

				return true;
			}
		}

	private :
		NodeData m_nodeData;
		SortedNode m_sortedNode;
	};
};

#endif // __ECS_GRAPH_H__