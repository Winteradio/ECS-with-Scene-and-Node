#ifndef __ECS_BASE_CONTAINER_H__
#define __ECS_BASE_CONTAINER_H__

#include <Reflection/include/Type/TypeMacro.h>

#include "Object/Object.h"

namespace ECS
{
	class BaseContainer : public Object
	{
		GENERATE(BaseContainer);

	public:
		BaseContainer() = default;
		virtual ~BaseContainer() = default;

	public:
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void Erase(const UUID& uuid) = 0;
	};
};

#endif // __ECS_BASE_CONTAINER_H__