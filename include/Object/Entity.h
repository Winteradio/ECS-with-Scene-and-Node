#ifndef __ECS_ENTITY_H__
#define __ECS_ENTITY_H__

#include <Container/include/HashSet.h>
#include <Reflection/include/Type/TypeInfo.h>

#include "Object/Object.h"
#include "Utils.h"

namespace ECS
{
	class Entity : public Object
	{
		public :
			using TypeData = wtr::HashSet<const Reflection::TypeInfo*>;

			Entity();
			Entity(const std::string& name);
			Entity(const Entity& other);
			Entity(Entity&& other);
			~Entity() = default;

			Entity& operator=(const Entity& other);
			Entity& operator=(Entity&& other);

			bool operator==(const Entity& other) const;
			bool operator!=(const Entity& other) const;

		public:
			void AddComponent(const Reflection::TypeInfo* componentType);
			void RemoveComponent(const Reflection::TypeInfo* componentType);
			bool HasComponent(const Reflection::TypeInfo* componentType) const;

			void AddNode(const Reflection::TypeInfo* nodeType);
			void RemoveNode(const Reflection::TypeInfo* nodeType);
			bool HasNode(const Reflection::TypeInfo* nodeType) const;

		public :
			template<typename T>
			void AddComponent()
			{
				static_assert(Utils::IsComponent<T>, "Invalid type for the component");

				const Reflection::TypeInfo* componentType = Reflection::TypeInfo::Get<T>();
				AddComponent(componentType);
			}

			template<typename T>
			void RemoveComponent()
			{
				static_assert(Utils::IsComponent<T>, "Invalid type for the component");

				const Reflection::TypeInfo* componentType = Reflection::TypeInfo::Get<T>();
				RemoveComponent(componentType);
			}

			template<typename T>
			bool HasComponent() const
			{
				static_assert(Utils::IsComponent<T>, "Invalid types for the component");

				const Reflection::TypeInfo* componentType = Reflection::TypeInfo::Get<T>();
				return HasComponent(componentType);
			}

			template<typename T>
			void AddNode()
			{
				static_assert(Utils::IsNode<T>, "Invalid type for the node");

				const Reflection::TypeInfo* nodeType = Reflection::TypeInfo::Get<T>();
				AddComponent(nodeType);
			}

			template<typename T>
			void RemoveNode()
			{
				static_assert(Utils::IsNode<T>, "Invalid type for the node");

				const Reflection::TypeInfo* nodeType = Reflection::TypeInfo::Get<T>();
				RemoveNode(nodeType);
			}

			template<typename T>
			bool HasNode() const
			{
				static_assert(Utils::IsNode<T>, "Invalid type for the node");

				const Reflection::TypeInfo* nodeType = Reflection::TypeInfo::Get<T>();
				return HasNode(nodeType);
			}

		public :
			const TypeData& GetComponentType() const;
			const TypeData& GetNodeType() const;
			const std::string& GetName() const;

		private :
			TypeData m_componentType;
			TypeData m_nodeType;
			std::string m_name;
	};
};

#endif // __ECS_ENTITY_H__