#ifndef __ECS_COMPONENT_REGISTRY_H__
#define __ECS_COMPONENT_REGISTRY_H__

#include "Container/ComponentContainer.h"
#include "Registry/ContainerRegistry.h"

namespace ECS
{
	class ComponentRegistry : public ContainerRegistry<ComponentContainer>
	{
		GENERATE(ComponentRegistry);

		public :
			using Base = ContainerRegistry<ComponentContainer>;
			using Base::ContainerRegistry;
	};
};

#endif // __ECS_COMPONENT_REGISTRY_H__