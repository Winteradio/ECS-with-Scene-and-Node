#ifndef __ECS_SYSTEM_H__
#define __ECS_SYSTEM_H__

#include <Reflection/include/Type/TypeInfo.h>
#include <Memory/include/Memory.h>

#include <Container/Container.h>
#include <TimeStep.h>

namespace ECS
{
	template<typename Base>
	class System : public Object
	{
		GENERATE(System);

	public:
		System() = default;
		virtual ~System() = default;

	public:
		virtual void Update(const TimeStep& timeStep, Memory::ObjectPtr<BaseContainer<Base>> container) = 0;
		virtual const Reflection::TypeInfo* GetNodeType() const = 0;
	};
};

#endif // __ECS_SYSTEM_H__