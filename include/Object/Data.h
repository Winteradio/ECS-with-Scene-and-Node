#ifndef __ECS_DATA_H__
#define __ECS_DATA_H__

#include "Object/Object.h"

#include <utility>

namespace ECS
{
	namespace Component
	{
		// Add the Component Type that inheritance the base
		struct Base {};
	};

	namespace Node
	{
		// Add the Node Type that inheritance the base
		struct Base : public Object {};
	};
};

#endif // __ECS_DATA_H__