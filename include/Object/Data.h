#ifndef __ECS_DATA_H__
#define __ECS_DATA_H__

#include "Object/Object.h"

#include <utility>

namespace ECS
{
	// Add the Component Type that inheritance the base
	struct Component : Object
	{
		using Object::Object;

		Component() = default;
		virtual ~Component() = default;
	};

	// Add the Node Type that inheritance the base
	struct Node : public Object 
	{
		using Object::Object;

		Node() = default;
		virtual ~Node() = default;
	};
};

#endif // __ECS_DATA_H__