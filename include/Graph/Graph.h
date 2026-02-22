#ifndef __ECS_GRAPH_H__
#define __ECS_GRAPH_H__

#include <Container/include/DynamicArray.h>
#include <Container/include/HashSet.h>
#include <Container/include/HashMap.h>

#include <Log/include/Log.h>

#include <queue>
#include <initializer_list>
#include <algorithm>

namespace ECS
{
	template<typename T>
	struct DefaultMaker
	{
		std::string operator()(const T& data) const
		{
			return std::string();
		}
	};

	template<typename T, typename StringMaker = DefaultMaker<T>>
	class Graph
	{
	public :
		using SortedNode = wtr::DynamicArray<T>;

		using NodeData = wtr::HashSet<T>;
		using NodeIndegree = wtr::HashMap<T, uint32_t>;
		using NodeOrder = wtr::HashMap<T, size_t>;
		using NodeEdge = wtr::HashMap<T, NodeData>;
		using NodeQueue = std::queue<T>;

		Graph()
			: m_edge()
			, m_order()
			, m_indegree()
			, m_sorted()
			, m_updated(false)
		{}

		~Graph() = default;

	public:
		void Add(const T& data)
		{
			auto [itr, inserted] = m_edge.TryEmplace(data);
			if (inserted)
			{
				m_indegree[data] = 0;

				m_updated = true;
			}
		}

		void Add(const T& from, const std::initializer_list<T>& list)
		{
			for (const auto& to : list)
			{
				Add(from, to);
			}
		}

		void Add(const T& from, const T& to)
		{
			m_edge[from];

			auto [itr, inserted] = m_edge[to].Insert(from);
			if (inserted)
			{
				m_indegree[from]++;
				m_indegree[to];

				m_updated = true;
			}
		}

		void Remove(const T& data)
		{
			m_sorted.Erase(data);
			m_order.Erase(data);
			m_indegree.Erase(data);
			m_edge.Erase(data);

			for (auto& edgePair : m_edge)
			{
				auto& nodeData = edgePair.second;

				nodeData.Erase(data);
			}
		}

		bool Build()
		{
			if (!m_updated)
			{
				return true;
			}

			wtr::DynamicArray<T> sortedList;
			sortedList.Reserve(m_indegree.Size());

			NodeIndegree indegree = m_indegree;
			NodeQueue queue;

			for (const auto& pair : indegree)
			{
				const auto& node = pair.first;
				const auto& count = pair.second;

				if (count == 0)
				{
					queue.push(node);
				}
			}

			while (!queue.empty())
			{
				T node = queue.front();
				queue.pop();

				const auto& dependData = m_edge[node];
				for (const auto& depend : dependData)
				{
					indegree[depend]--;

					if (indegree[depend] == 0)
					{
						queue.push(depend);
					}
				}

				sortedList.PushBack(node);
			}

			if (sortedList.Size() != m_indegree.Size())
			{
				LOGINFO() << "[GRAPH] Failed to build, cause the circular loop occured";
				
				for (const auto& nodePair : m_edge)
				{
					const auto& node = nodePair.first;
					const auto& dependData = nodePair.second;

					auto itr = sortedList.Find(node);
					if (itr != sortedList.End())
					{
						continue;
					}
					
					for (const auto& dependNode : dependData)
					{
						LOGINFO() << "[GRAPH] Expected the circular : "
							<< StringMaker()(dependNode) << " -> " << StringMaker()(node);
					}
				}

				return false;
			}
			else
			{
				m_sorted = std::move(sortedList);

				const size_t endIndex = m_sorted.Size();
				for (size_t index = 0; index < endIndex; index++)
				{
					const auto& node = m_sorted[index];
					m_order[node] = index;
				}

				m_updated = false;

				return true;
			}
		}

		bool IsUpdated() const
		{
			return m_updated;
		}

		const SortedNode GetPartialSorted(const wtr::DynamicArray<T>& otherList) const
		{
			wtr::DynamicArray<T> partial = otherList;

			auto sortFunction = [order = m_order](const T& lhs, const T& rhs)
			{
				const size_t lhsOrder = order[lhs];
				const size_t rhsOrder = order[rhs];

				return lhsOrder < rhsOrder;
			};

			partial.Sort(partial.Begin(), partial.End(), sortFunction);

			return partial;
		}

		const SortedNode& GetSorted() const
		{
			return m_sorted;
		}

	private :
		NodeEdge m_edge;
		NodeOrder m_order;
		NodeIndegree m_indegree;

		SortedNode m_sorted;

		bool m_updated;
	};
};

#endif // __ECS_GRAPH_H__