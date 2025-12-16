#ifndef __ECS_TIME_STEP_H__
#define __ECS_TIME_STEP_H__

#include <cstddef>

namespace ECS
{
	struct TimeStep
	{
		double total;
		double delta;
		double scale;
		uint32_t frame;
	};
};

#endif // __ECS_TIME_STEP_H__