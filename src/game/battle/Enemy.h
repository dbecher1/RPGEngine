//
// Created by Daniel Becher on 3/6/24.
//

#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "Attributes.h"

struct Enemy {
    std::string name;
    Attributes stats{};
};

#endif //ENEMY_H
