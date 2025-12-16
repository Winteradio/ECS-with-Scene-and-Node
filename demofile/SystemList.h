#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <Log/include/Log.h>

#include "NodeList.h"
#include "Object/System.h"

class RenderSystem : public ECS::TypedSystem<RenderNode>
{
	GENERATE(RenderSystem);

public :
	void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		LOGINFO() << "[TEST] System(" << GetTypeInfo()->GetTypeName() << ") Update";

		for (auto& [uuid, node] : container->GetStorage())
		{
			if (node && node->color && node->resource && node->transform)
			{
				LOGINFO() << "[TEST] RenderNode"
					<< " Color(" << node->color->Red << ", " << node->color->Green << ", " << node->color->Blue << ", " << node->color->Alpha << ")"
					<< " Resource(" << node->resource->resourceID << ")";
			}
		}

		(void)timeStep;
		(void)container;
	}
};

class GravitySystem : public ECS::TypedSystem<PhysicsNode>
{
	GENERATE(GravitySystem);

public:
	void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		LOGINFO() << "[TEST] System(" << GetTypeInfo()->GetTypeName() << ") Update";

		(void)timeStep;
		(void)container;
	}
};

class MoveSystem : public ECS::TypedSystem<PhysicsNode>
{
	GENERATE(MoveSystem);

public:
	void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		LOGINFO() << "[TEST] System(" << GetTypeInfo()->GetTypeName() << ") Update";

		(void)timeStep;
		(void)container;
	}
};

class CollisionSystem : public ECS::TypedSystem<CollisionNode>
{
	GENERATE(CollisionSystem);

public:
	void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		LOGINFO() << "[TEST] System(" << GetTypeInfo()->GetTypeName() << ") Update";

		(void)timeStep;
		(void)container;
	}
};

class CameraSystem : public ECS::TypedSystem<CameraNode>
{
	GENERATE(CameraSystem);

public:
	void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		LOGINFO() << "[TEST] System(" << GetTypeInfo()->GetTypeName() << ") Update";

		(void)timeStep;
		(void)container;
	}
};

#endif // __SYSTEM_H__