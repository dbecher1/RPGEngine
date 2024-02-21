//
// Created by Daniel Becher on 2/20/24.
//

#ifndef RPG_MISCMATH_H
#define RPG_MISCMATH_H

#include "../game/misc/macros.h"

inline bool float_eq(float f1, float f2) {
    return std::abs(f1 - f2) <= EPSILON;
}

#endif //RPG_MISCMATH_H
