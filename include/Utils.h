#ifndef __ECS_UTILS_H__
#define __ECS_UTILS_H__

#include <Reflection/include/Utils.h>

namespace ECS
{
	struct Component;
	struct Node;
	
	class System;
	class Object;
	
	namespace Utils
	{
		template<typename T>
		constexpr bool IsComponent = Reflection::Utils::IsBase<Component, T>::value;

		template<typename T>
		constexpr bool IsNode = Reflection::Utils::IsBase<Node, T>::value;

		template<typename T>
		constexpr bool IsSystem = Reflection::Utils::IsBase<System, T>::value;

		template<typename T>
		constexpr bool IsObject = Reflection::Utils::IsBase<Object, T>::value;
	};
};

#endif // __ECS_UTILS_H__