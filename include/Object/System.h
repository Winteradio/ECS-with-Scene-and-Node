#ifndef __ECS_SYSTEM_H__
#define __ECS_SYSTEM_H__

#include <Reflection/include/Type/TypeInfo.h>
#include <Memory/include/Memory.h>

#include "Object/Object.h"
#include "TimeStep.h"
#include "Utils.h"

namespace ECS
{
	class BaseContainer;

	template<typename T>
	class NodeContainer;

	class System : public Object
	{
		GENERATE(System);

		public :
			using DependData = wtr::HashSet<UUID>;

			System(const Reflection::TypeInfo* nodeType);
			System(const System& other);
			System(System&& other);

			System& operator=(const System& other);
			System& operator=(System&& other) noexcept;

			bool operator==(const System& other) const;
			bool operator!=(const System& other) const;

			virtual ~System() = default;

		public : 
			virtual void Update(const TimeStep& timeStep, Memory::ObjectPtr<BaseContainer> container) = 0;
			
			void DependOn(const UUID& systemID);
			void DependOn(const Memory::ObjectPtr<System> system);

			void RemoveOn(const UUID& systemID);
			void RemoveOn(const Memory::ObjectPtr<System> system);

			const DependData& GetDependData() const;
			const Reflection::TypeInfo* GetNodeType() const;
			
		protected :
			DependData m_depend;
			const Reflection::TypeInfo* m_nodeType;
	};

	template<typename Node>
	class TypedSystem : public System
	{
		static_assert(Utils::IsNode<Node> && "Invalid the node type");

		GENERATE(TypedSystem);

		public :
			using System::System;
			using ContainerType = NodeContainer<Node>;
			using NodeType = Node;
			
			TypedSystem()
				: System(Reflection::TypeInfo::Get<Node>())
			{}

		public :
			void Update(const TimeStep& timeStep, Memory::ObjectPtr<BaseContainer> container) override
			{
				Memory::ObjectPtr<ContainerType> nodeContainer = Memory::Cast<ContainerType>(container);
				if (nodeContainer)
				{
					UpdateInternal(timeStep, nodeContainer);
				}
			}

		protected:
			virtual void UpdateInternal(const TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) = 0;
	};
}

// TypedSystem

#endif // __ECS_SYSTEM_H__