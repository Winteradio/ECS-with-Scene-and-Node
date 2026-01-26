#ifndef __ECS_SCENE_H__
#define __ECS_SCENE_H__

#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>
#include <Container/include/HashSet.h>
#include <Log/include/Log.h>
#include <Reflection/include/Type/TypeInfo.h>
#include <Memory/include/Memory.h>

#include "Utils.h"
#include "UUID/UUID.h"

#include <string>

namespace ECS
{
	class Scene
	{
		public :
			using IDArray = wtr::DynamicArray<UUID>;
			using IDSet = wtr::HashSet<UUID>;

			using TypeData = wtr::HashMap<size_t, uint32_t>;

			Scene();
			Scene(const std::string& name);
			virtual ~Scene() = default;

		public :
			template<typename T>
			void RegisterNode(const Memory::ObjectPtr<T> node)
			{
				static_assert(Utils::IsNode<T> && "Invalid Node");

				if (!node)
				{
					return;
				}

				const UUID& nodeId = node->GetID();
				m_nodeID.Insert(nodeId);

				const Reflection::TypeInfo* nodeType = Reflection::TypeInfo::Get<T>();
				const size_t typeHash = nodeType->GetTypeHash();
				m_nodeType[typeHash]++;
			}

			template<typename T>
			void RemoveNode(const Memory::ObjectPtr<T> node)
			{
				static_assert(Utils::IsNode<T> && "Invalid Node");

				if (!node)
				{
					return;
				}

				const UUID& nodeId = node->GetID();
				m_nodeID.Erase(nodeId);

				const Reflection::TypeInfo* nodeType = Reflection::TypeInfo::Get<T>();
				const size_t typeHash = nodeType->GetTypeHash();

				auto itr = m_nodeType.Find(typeHash);
				if (itr != m_nodeType.End())
				{
					uint32_t& typeCount = itr->second;
					typeCount--;

					if (0 == typeCount)
					{
						m_nodeType.Erase(itr);
					}
				}
			}

			template<typename T>
			bool HasNodeType()
			{
				static_assert(Utils::IsNode<T> && "Invalid Node");

				const Reflection::TypeInfo* nodeType = Reflection::TypeInfo::Get<T>();

				return HasNodeType(nodeType);
			}

			template<typename T>
			void RegisterSystem(const Memory::ObjectPtr<T> system)
			{
				static_assert(Utils::IsSystem<T> && "Invalid System");

				if (!system)
				{
					return;
				}

				if (HasNodeType<typename T::NodeType>())
				{
					const UUID& systemId = system->GetID();

					m_systemID.PushBack(systemId);
				}
				else
				{
					const Reflection::TypeInfo* systemType = Reflection::TypeInfo::Get<T>();
					const Reflection::TypeInfo* nodeType = Reflection::TypeInfo::Get<typename T::NodeType>();
					LOGINFO() << "[SCENE] Failed to register the system(" << systemType->GetTypeName() << "), the node(" << nodeType->GetTypeName() << ") isn't existed the scene(" << this << ")";
				}
			}

			template<typename T>
			void RemoveSystem(const Memory::ObjectPtr<T> system)
			{
				static_assert(Utils::IsSystem<T> && "Invalid System");

				if (!system)
				{
					return;
				}

				const UUID& systemId = system->GetID();

				auto itr = m_systemID.Find(systemId);
				m_systemID.Erase(itr);
			}

		public :
			bool HasNode(const UUID& nodeId) const;
			bool HasNodeType(const Reflection::TypeInfo* nodeType) const;
			bool HasSystem(const UUID& systemId) const;
			
			const TypeData& GetNodeType() const;
			const IDSet& GetNodeID() const;
			const IDArray& GetSystemID() const;
			const std::string& GetName() const;

			void UpdateSystemID(const IDArray& systemID);
			void SetName(const std::string& name);

		private :
			TypeData m_nodeType;

			IDSet m_nodeID;
			IDArray m_systemID;

			std::string m_name;
	};
};
#endif // __ECS_SCENE_H__