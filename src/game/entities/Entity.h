//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_ENTITY_H
#define RPG_ENTITY_H

#include "../gfx/DrawCommand.h"

class Entity {
public:
    virtual void Update(unsigned long dt) = 0;

};


#endif //RPG_ENTITY_H
