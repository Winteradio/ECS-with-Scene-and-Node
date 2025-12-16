#include "Object/Object.h"

namespace ECS
{
	Object::Object()
		: m_uuid()
	{}
	
	Object::Object(const UUID& uuid)
		: m_uuid(uuid)
	{}

	Object::Object(const Object& other)
		: m_uuid(other.m_uuid)
	{}

	Object::Object(Object&& other)
		: m_uuid(std::move(other.m_uuid))
	{}

	Object& Object::operator=(const Object& other)
	{
		if (this != &other)
		{
			m_uuid = other.m_uuid;
		}
		
		return *this;
	}

	Object& Object::operator=(Object&& other)
	{
		if (this != &other)
		{
			m_uuid = std::move(other.m_uuid);
		}

		return *this;
	}

	bool Object::operator==(const Object& other) const
	{
		return m_uuid == other.m_uuid;
	}

	bool Object::operator!=(const Object& other) const
	{
		return !(*this == other);
	}

	void Object::SetID(const UUID& uuid)
	{
		m_uuid = uuid;
	}

	const UUID& Object::GetID() const
	{
		return m_uuid;
	}
};