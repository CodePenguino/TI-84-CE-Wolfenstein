#pragma once

#include <sys/timers.h>
#define BENCHMARK_ENABLED 1
#define BENCHMARK_TIMER 3

// Variables for benchmarking
#if BENCHMARK_ENABLED
//static uint16_t now_time;
//static uint16_t last_time;
//static uint16_t delta_time;

static uint32_t benchmark_start_time;
static uint32_t benchmark_stop_time;

static inline void debug_enable()
{
	timer_Enable(BENCHMARK_TIMER, TIMER_CPU, TIMER_0INT, TIMER_UP);
}

static inline void debug_disable()
{
	timer_Disable(BENCHMARK_TIMER);
}

static inline uint32_t debug_get()
{
	return timer_GetSafe(BENCHMARK_TIMER, TIMER_UP);
}

#define benchmark_start() (benchmark_start_time = debug_get())
#define benchmark_stop() (benchmark_stop_time = debug_get())

#define benchmark_func(x) ({                                 \
	benchmark_start();                                         \
	x;                                                         \
	benchmark_stop();                                          \
	uint32_t ret = benchmark_stop_time - benchmark_start_time; \
	ret;                                                       \
})

#else
static inline void debug_enable() {}
static inline void debug_disable() {}
static inline uint32_t debug_get() { return 69; } // Nice
#define benchmark_start() ({})
#define benchmark_stop() ({})
#define benchmark_func(x) ({x; 69;})
#endif
