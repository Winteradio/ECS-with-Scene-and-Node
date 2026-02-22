#ifndef __ECS_UUID_H__
#define __ECS_UUID_H__

#include <functional>
#include <random>

#include <Container/include/StaticArray.h>

namespace ECS
{
	class UUID
	{
	public:
		UUID();
		UUID(const UUID& other);
		UUID(UUID&& other);

		UUID& operator=(const UUID& other);
		UUID& operator=(UUID&& other);

		bool operator==(const UUID& other) const;
		bool operator!=(const UUID& other) const;

		const std::string ToString() const;

		bool IsValid() const;

	private:
		friend class std::hash<ECS::UUID>;

		uint64_t m_high;
		uint64_t m_low;
	};

	struct UUIDString
	{
		std::string operator()(const UUID& uuid) const
		{
			return uuid.ToString();
		}
	};
};

namespace std
{
	template<>
	struct hash<ECS::UUID>
	{
		std::size_t operator()(const ECS::UUID& uuid) const noexcept
		{
			return uuid.m_high ^ uuid.m_low;
		}
	};
};

#endif // __ECS_UUID_H__