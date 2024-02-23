//
// Created by Daniel Becher on 2/18/24.
//

#include "Timer.h"
#include "../game/misc/macros.h"

#if USE_SDL_TIMERS

#include "SDL_timer.h"

#define CURRENT_DECL Uint64 current;

#define PREV_DECL Uint64 previous = SDL_GetPerformanceCounter()

#define GET_TIME() (SDL_GetPerformanceCounter())

#define CALCULATE_TIME(current, previous) (static_cast<double>(current - previous) / static_cast<double>(SDL_GetPerformanceFrequency()))

#else

#include <chrono>

#define CURRENT_DECL std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> current

#define PREV_DECL std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> previous = std::chrono::steady_clock::now()

#define GET_TIME() (std::chrono::steady_clock::now())

#define CALCULATE_TIME(current, previous) (static_cast<double>((current - previous).count() * 0.000000001))

#endif

CURRENT_DECL;
PREV_DECL;

void Timer::tick() {
    current = GET_TIME();
    dt = CALCULATE_TIME(current, previous);
    previous = current;
    std::cout << dt << std::endl;
}
