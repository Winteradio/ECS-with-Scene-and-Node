#ifndef __ECS_TIME_STEP_H__
#define __ECS_TIME_STEP_H__

#include <cstddef>
#include <chrono>

namespace ECS
{
	struct TimeStep
	{
		using Clock = std::chrono::high_resolution_clock;
		using Duration = std::chrono::milliseconds;

		Clock::time_point start = Clock::now();
		Duration total = Duration::zero();
		Duration delta = Duration::zero();
		double scale = 1.0;
		uint32_t frame = 0;

		void Tick()
		{
			auto now = Clock::now();
			auto duration = std::chrono::duration_cast<Duration>(now - start);
			delta = duration;
			total += duration;
			frame++;
		}

		static const double ToSecond(const Duration& duration)
		{
			return static_cast<double>(duration.count()) / 1000.0;
		}

		static const double ToMilliSecond(const Duration& duration)
		{
			return static_cast<double>(duration.count());
		}
	};
};

#endif // __ECS_TIME_STEP_H__