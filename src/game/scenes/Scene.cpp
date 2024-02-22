//
// Created by Daniel Becher on 2/7/24.
//

#include "Scene.h"

Scene::Scene() {

}

void Scene::Update(double dt) {
    for (auto &e : activeEntities) {
        e.Update(dt);
    }
}
