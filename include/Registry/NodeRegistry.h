#ifndef __ECS_NODE_REGISTRY_H__
#define __ECS_NODE_REGISTRY_H__

#include "Container/NodeContainer.h"
#include "Registry/ContainerRegistry.h"

namespace ECS
{
	class NodeRegistry : public ContainerRegistry<NodeContainer>
	{
		GENERATE(NodeRegistry);

		public :
			using Base = ContainerRegistry<NodeContainer>;
			using Base::ContainerRegistry;
	};
};

#endif // __ECS_NODE_REGISTRY_H__