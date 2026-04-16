#ifndef __ECS_DATA_H__
#define __ECS_DATA_H__

#include <ECS/include/Object/Object.h>

namespace ECS
{
	// Add the Component Type that inheritance the base
	struct Component : Object
	{
	public :
		Component() = default;
		virtual ~Component() = default;
	};

	// Add the Node Type that inheritance the base
	struct Node : Object
	{
	public :
		Node() = default;
		virtual ~Node() = default;
	};
};

#endif // __ECS_DATA_H__