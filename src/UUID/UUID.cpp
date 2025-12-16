#include "UUID/UUID.h"

namespace ECS
{
	UUID::UUID()
	{
		static std::random_device device;
		static std::mt19937_64 generator(device());

		m_high = (generator() & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
		m_low = (generator() & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;
	}

	UUID::UUID(const UUID& other)
		: m_high(other.m_high)
		, m_low(other.m_low)
	{}

	UUID::UUID(UUID&& other)
		: m_high(other.m_high)
		, m_low(other.m_low)
	{
		other.m_high = 0x00;
		other.m_low = 0x00;
	}

	UUID& UUID::operator=(const UUID& other)
	{
		if (this != &other)
		{
			m_high = other.m_high;
			m_low = other.m_low;
		}

		return *this;
	}

	UUID& UUID::operator=(UUID&& other)
	{
		if (this != &other)
		{
			m_high = other.m_high;
			m_low = other.m_low;

			other.m_high = 0x00;
			other.m_low = 0x00;
		}

		return *this;
	}

	bool UUID::operator==(const UUID& other) const
	{
		return m_high == other.m_high && m_low == other.m_low;
	}

	bool UUID::operator!=(const UUID& other) const
	{
		return !(*this == other);
	}

	const std::string UUID::ToString() const
	{
		static wtr::StaticArray<uint8_t, 5> rangeArray = { 8, 4, 4, 4, 12 };

		std::string uuid;

		constexpr uint8_t maxHex = 32;
		constexpr uint8_t maxHalfHex = maxHex / 2;
		uint8_t hexCount = 0;

		uint8_t rangeIndex = 0;
		uint8_t rangeSum = rangeArray[rangeIndex];

		while (hexCount < 32)
		{
			const uint64_t data = hexCount >= maxHalfHex ? m_low : m_high;
			const uint8_t hexIndex = hexCount % 16;
			const uint8_t shiftValue = (maxHalfHex - (hexIndex + 1)) * 4;

			const uint8_t postData = (data >> shiftValue) & 0x0F;

			if (postData >= 10)
			{
				constexpr uint8_t upperASCII = 65;
				uuid += upperASCII + (postData - 10);
			}
			else
			{
				constexpr uint8_t numberASCII = 48;
				uuid += numberASCII + postData;
			}

			if (rangeIndex < (rangeArray.Size() - 1) && rangeSum == (hexCount + 1))
			{
				uuid += "-";

				rangeIndex++;
				rangeSum += rangeArray[rangeIndex];
			}

			hexCount++;
		}

		return uuid;
	}

	bool UUID::IsValid() const
	{
		return m_high != 0x00 || m_low != 0x00;
	}
}