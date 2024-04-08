#pragma once

#include <sys/timers.h>
#define BENCHMARK_ENABLED 1
#define BENCHMARK_TIMER 3

static uint24_t benchmark_start_time;
static uint24_t benchmark_stop_time;
// Variables for benchmarking
#if BENCHMARK_ENABLED

static inline void benchmark_enable()
{
	timer_Enable(BENCHMARK_TIMER, TIMER_CPU, TIMER_0INT, TIMER_UP);
}

static inline void benchmark_disable()
{
	timer_Disable(BENCHMARK_TIMER);
}

static inline uint24_t benchmark_get_time()
{
	return timer_GetSafe(BENCHMARK_TIMER, TIMER_UP);
}

#define benchmark_start() (benchmark_start_time = benchmark_get_time())
#define benchmark_stop() (benchmark_stop_time = benchmark_get_time())

#define benchmark_func(x) ({                                 \
	benchmark_start();                                         \
	x;                                                         \
	benchmark_stop();                                          \
	uint24_t ret = benchmark_stop_time - benchmark_start_time; \
	ret;                                                       \
})

#define benchmark_get_delta() benchmark_stop_time - benchmark_start_time

#else
#define benchmark_enable() 0
#define benchmark_disable() 0
#define benchmark_get_time() 69
#define benchmark_start() 0
#define benchmark_stop() 0
#define benchmark_func(x) ({x; 69;})
#endif
