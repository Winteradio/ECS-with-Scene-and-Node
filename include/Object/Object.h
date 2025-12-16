#ifndef __ECS_Object_H__
#define __ECS_Object_H__

#include "UUID/UUID.h"

#include <utility>

namespace ECS
{
	class Object
	{
		public :
			Object();
			Object(const UUID& uuid);
			Object(const Object& other);
			Object(Object&& other);
			~Object() = default;

			Object& operator=(const Object& other);
			Object& operator=(Object&& other);

			bool operator==(const Object& other) const;
			bool operator!=(const Object& other) const;

		public :
			void SetID(const UUID& uuid);
			const UUID& GetID() const;
			
		protected :
			UUID m_uuid;
	};
};

#endif // __ECS_Object_H__