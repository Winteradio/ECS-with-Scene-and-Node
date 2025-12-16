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
			using TypeData = wtr::HashSet<size_t>;

			Entity();
			Entity(const std::string& name);
			Entity(const Entity& other);
			Entity(Entity&& other);
			~Entity() = default;

			Entity& operator=(const Entity& other);
			Entity& operator=(Entity&& other);

			bool operator==(const Entity& other) const;
			bool operator!=(const Entity& other) const;

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

		public :
			void AddComponent(const Reflection::TypeInfo* componentType);
			void RemoveComponent(const Reflection::TypeInfo* componentType);
			bool HasComponent(const Reflection::TypeInfo* componentType) const;

		public :
			const TypeData& GetTypeData() const;
			const std::string& GetName() const;

		private :
			TypeData m_componentType;
			std::string m_name;
	};
};

#endif // __ECS_ENTITY_H__