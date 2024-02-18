//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include <vector>
#include "../entities/Entity.h"
#include "Environment.h"

class Scene {
public:
    Scene();
    void Update(unsigned long dt);

private:
    bool isActive = true;
    std::vector<Entity> activeEntities;
    Environment environment;
};


#endif //RPG_SCENE_H
