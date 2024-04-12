#pragma once

#include <sys/timers.h>
#include <stdint.h>

#define SYSTEM_TIMER 1

static inline void time_enable()
{
	timer_Enable(SYSTEM_TIMER, TIMER_32K, TIMER_0INT, TIMER_UP);
}

static inline void time_disable()
{
	timer_Disable(SYSTEM_TIMER);
}

static inline uint24_t time_get()
{
	return timer_GetSafe(SYSTEM_TIMER, TIMER_UP);
}

//#define time_get_fps() (32768 / time_delta)

static inline uint24_t time_get_fps()
{
	return 32768 / time_get();
}
