#pragma once

#include <sys/timers.h>

#define SYSTEM_TIMER 1

static uint16_t time_now;
static uint16_t time_last;
static uint16_t time_delta;

static inline void time_enable()
{
	timer_Enable(SYSTEM_TIMER, TIMER_32K, TIMER_0INT, TIMER_UP);
}

static inline void time_disable()
{
	timer_Disable(SYSTEM_TIMER);
}

static inline uint16_t time_get()
{
	return timer_Get(SYSTEM_TIMER);
}

#define time_get_fps() (32768 / time_delta)

/*static inline uint16_t time_get_fps()
{
	return 32768 / time_delta;
}*/

static inline void time_update()
{
	time_now = time_get();
	time_delta = time_now - time_last;
	time_last = time_now;
}
