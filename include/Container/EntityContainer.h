#ifndef __ECS_ENTITY_CONTAINER_H__
#define __ECS_ENTITY_CONTAINER_H__

#include <Container/include/HashMap.h>
#include <Memory/include/Memory.h>
#include <Reflection/include/Property/PropertyMacro.h>

#include "Container/BaseContainer.h"
#include "Object/Entity.h"
#include "Utils.h"

namespace ECS
{
	class EntityContainer : public BaseContainer
	{
		GENERATE(EntityContainer);

		public :
			using Storage = wtr::HashMap<UUID, Memory::ObjectPtr<Entity>>;

			EntityContainer();
			EntityContainer(const EntityContainer& other);
			EntityContainer(EntityContainer&& other);

			EntityContainer& operator=(const EntityContainer& other);
			EntityContainer& operator=(EntityContainer&& other);

			virtual ~EntityContainer() = default;

			bool operator==(const EntityContainer& other);
			bool operator!=(const EntityContainer& other);

		public :
			void Init() override;
			void Clear() override;
			void Erase(const UUID& uuid) override;

			Memory::ObjectPtr<Entity> Emplace(const std::string& name);

		public :
			Memory::ObjectPtr<Entity> Get(const UUID& uuid);
			const Memory::ObjectPtr<Entity> Get(const UUID& uuid) const;

		private :
			PROPERTY(m_storage);
			Storage m_storage;
	};
};

#endif // __ECS_ENTITY_CONTAINER_H__