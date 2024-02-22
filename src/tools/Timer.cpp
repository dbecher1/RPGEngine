//
// Created by Daniel Becher on 2/18/24.
//

#include "Timer.h"

void Timer::tick() {
    if (flip) {
        current_1 = SDL_GetPerformanceCounter();
        dt_1 = static_cast<double>(current_1 - previous_1) / static_cast<double>(SDL_GetPerformanceFrequency());
        previous_1 = current_1;
    }
    else {
        current_2 = SDL_GetPerformanceCounter();
        dt_2 = static_cast<double>(current_2 - previous_2) / static_cast<double>(SDL_GetPerformanceFrequency());
        previous_2 = current_2;
    }
    flip = !flip;
    dt = (dt_1 + dt_2) * 0.5;
}
