//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include <vector>
#include "../entities/Entity.h"

class Scene {
public:
    Scene();
    void Update(double dt);

private:
    bool isActive = true;
    std::vector<Entity> activeEntities;
};


#endif //RPG_SCENE_H
