//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_SCENEMANAGER_H
#define RPG_SCENEMANAGER_H

#include <stack>
#include "scenes/Scene.h"

// UI goes here??

class SceneManager {

public:
    void Update(unsigned long dt);

private:
    std::stack<Scene> SceneStack;
};


#endif //RPG_SCENEMANAGER_H
