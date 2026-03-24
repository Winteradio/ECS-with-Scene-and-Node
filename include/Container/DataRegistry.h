#ifndef __ECS_DATAREGISTRY_H__
#define __ECS_DATAREGISTRY_H__

#include <Container/Registry.h>
#include <Object/Data.h>

namespace ECS
{
	using ComponentRegistry = Registry<Component>;
	using NodeRegistry = Registry<Node>;
};

#endif // __ECS_DATAREGISTRY_H__