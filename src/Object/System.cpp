#include "Object/System.h"

namespace ECS
{
	System::System(const Reflection::TypeInfo* nodeType)
		: Object()
		, m_nodeType(nodeType)
		, m_depend()
	{}

	System::System(const System& other)
		: Object(other)
		, m_nodeType(other.m_nodeType)
		, m_depend()
	{}

	System::System(System&& other)
		: Object(std::move(other))
		, m_nodeType(other.m_nodeType)
		, m_depend(std::move(other.m_depend))
	{
		other.m_nodeType = nullptr;
	}

	System& System::operator=(const System& other)
	{
		if (this != &other)
		{
			Object::operator=(other);

			m_nodeType = other.m_nodeType;
			m_depend = other.m_depend;
		}

		return *this;
	}

	System& System::operator=(System&& other) noexcept
	{
		if (this != &other)
		{
			Object::operator=(std::move(other));

			m_nodeType = other.m_nodeType;
			m_depend = other.m_depend;

			other.m_nodeType = nullptr;
		}

		return *this;
	}

	bool System::operator==(const System& other) const
	{
		return Object::operator==(other);
	}

	bool System::operator!=(const System& other) const
	{
		return !(*this == other);
	}

	void System::DependOn(const UUID& systemID)
	{
		m_depend.Insert(systemID);
	}

	void System::DependOn(const Memory::ObjectPtr<System> system)
	{
		if (system)
		{
			m_depend.Insert(system->GetID());
		}
	}

	void System::RemoveOn(const UUID& systemID)
	{
		m_depend.Erase(systemID);
	}

	void System::RemoveOn(const Memory::ObjectPtr<System> system)
	{
		if (system)
		{
			m_depend.Erase(system->GetID());
		}
	}

	const System::DependData& System::GetDependData() const
	{
		return m_depend;
	}

	const Reflection::TypeInfo* System::GetNodeType() const
	{
		return m_nodeType;
	}
}