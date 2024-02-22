//
// Created by Daniel Becher on 2/18/24.
//

#ifndef RPG_TIMER_H
#define RPG_TIMER_H

#include <SDL_timer.h>

struct Timer {
    void tick();
    double dt{};
private:
    Uint64 current_1{};
    Uint64 previous_1{};
    Uint64 current_2{};
    Uint64 previous_2{};
    double dt_1{};
    double dt_2{};
    bool flip{};
};


#endif //RPG_TIMER_H
