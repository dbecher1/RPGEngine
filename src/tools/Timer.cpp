//
// Created by Daniel Becher on 2/18/24.
//

#include "Timer.h"

void Timer::tick() {
    const Uint64 current_ticks = SDL_GetPerformanceCounter();
    const Uint64 delta = current_ticks - previous_ticks;
    previous_ticks = current_ticks;
    static const Uint64 TICKS_PER_SECOND = SDL_GetPerformanceFrequency();
    elapsed_seconds = static_cast<float>(delta) / static_cast<float>(TICKS_PER_SECOND);
}
