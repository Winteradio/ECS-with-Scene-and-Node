#ifndef __ECS_STORAGE_H__
#define __ECS_STORAGE_H__

#include <Reflection/include/Macro.h>
#include <Container/include/HashMap.h>
#include <Memory/include/Pointer/ObjectPtr.h>

#include "UUID/UUID.h"

namespace ECS
{
	template<typename T>
	class ObjectStorage
	{
		GENERATE(ObjectStorage);

	public:
		using Type = T;
		using Data = wtr::HashMap<UUID, Memory::ObjectPtr<Type>>;
		using DataIter = typename Data::Iterator;
		using DataConstIter = typename Data::ConstIterator;

		ObjectStorage() = default;
		~ObjectStorage() = default;

	public:
		bool operator==(const ObjectStorage& other)
		{
			return m_data == other.m_data;
		}

		bool operator!=(const ObjectStorage& other)
		{
			return !(*this == other);
		}

	public:
		void Clear()
		{
			m_data.Clear();
		}

		void Erase(const UUID& uuid)
		{
			m_data.Erase(uuid);
		}

		Memory::ObjectPtr<Type> Get(const UUID& uuid)
		{
			auto itr = m_data.Find(uuid);
			if (itr != m_data.End())
			{
				return itr->second;
			}
			else
			{
				return {};
			}
		}

		Memory::ObjectPtr<const Type> Get(const UUID& uuid) const
		{
			auto itr = m_data.Find(uuid);
			if (itr != m_data.End())
			{
				return itr->second;
			}
			else
			{
				return {};
			}
		}

		template<typename... Args>
		Memory::ObjectPtr<Type> Emplace(Args&&... args)
		{
			static_assert(Reflection::Utils::IsBase<ECS::Object, Type>::value, "The value type must be derived from ECS::Object.");

			Memory::ObjectPtr<Type> object = Memory::MakePtr<Type>(std::forward<Args>(args)...);
			if (object)
			{
				m_data[object->GetID()] = object;
			}
			return object;
		}

	public:
		DataIter begin()
		{
			return m_data.begin();
		}

		DataIter end()
		{
			return m_data.end();
		}

		DataConstIter begin() const
		{
			return m_data.begin();
		}

		DataConstIter end() const
		{
			return m_data.end();
		}

	private:
		PROPERTY(m_data);
		Data m_data;
	};

	template<typename T>
	class DataStorage
	{
		GENERATE(DataStorage);
	public :
		using Type = T;
		using Data = wtr::HashMap<UUID, Memory::ObjectPtr<Type>>;
		using DataIter = typename Data::Iterator;
		using DataConstIter = typename Data::ConstIterator;

	public :
		void Clear()
		{
			m_data.Clear();
		}

		void Erase(const UUID& uuid)
		{
			m_data.Erase(uuid);
		}

		Memory::ObjectPtr<Type> Get(const UUID& uuid)
		{
			auto itr = m_data.Find(uuid);
			if (itr != m_data.End())
			{
				return itr->second;
			}
			else
			{
				return {};
			}
		}

		Memory::ObjectPtr<const Type> Get(const UUID& uuid) const
		{
			auto itr = m_data.Find(uuid);
			if (itr != m_data.End())
			{
				return itr->second;
			}
			else
			{
				return {};
			}
		}

		template<typename... Args>
		Memory::ObjectPtr<Type> Emplace(const UUID& uuid, Args&&... args)
		{
			static_assert(Reflection::Utils::IsBase<ECS::Object, Type>::value, "The value type must be derived from ECS::Object.");

			Memory::ObjectPtr<Type> object = Memory::MakePtr<Type>(std::forward<Args>(args)...);
			if (object)
			{
				object->SetID(uuid);
				m_data[uuid] = object;
			}

			return object;
		}

	public:
		DataIter begin()
		{
			return m_data.begin();
		}

		DataIter end()
		{
			return m_data.end();
		}

		DataConstIter begin() const
		{
			return m_data.begin();
		}

		DataConstIter end() const
		{
			return m_data.end();
		}

	private :
		PROPERTY(m_data);
		Data m_data;
	};
};

#endif // __ECS_STORAGE_H__