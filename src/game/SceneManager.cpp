//
// Created by Daniel Becher on 2/7/24.
//

#include "SceneManager.h"

void SceneManager::Update(double dt) {
    SceneStack.top().Update(dt);
}
