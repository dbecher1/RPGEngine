//
// Created by Daniel Becher on 2/18/24.
//

#ifndef RPG_TIMER_H
#define RPG_TIMER_H

#include <SDL_timer.h>

struct Timer {
    Uint64 previous_ticks{};
    float elapsed_seconds{};
    void tick();
};


#endif //RPG_TIMER_H
