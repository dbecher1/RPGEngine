//
// Created by Daniel Becher on 2/7/24.
//

#include "Scene.h"
#include "../ResourceManager.h"
#include "../SpriteBatch.h"

Scene::Scene(const SceneBuilder& sb, ResourceManager* rm) :
sceneName(sb.name) {
    if (!sb.is_active)
        isActive = false;
    for (const auto& s : sb.entities) {
        activeEntities.push_back(rm->getEntity(s));
    }
    map = rm->getMap(sb.map_name);
}

void Scene::Update(double dt) {
    for (auto &e : activeEntities) {
        e->Update(dt);
    }
}

void Scene::AddEntity(const std::string &name) {
    // TODO
}

void Scene::Draw(SpriteBatch *sb) const {
    map->Draw(sb);
    for (const auto &e : activeEntities) {
        DrawCommand dc;
        e->Draw(&dc);
        sb->Add(dc);
    }
}

void Scene::FixedUpdate() {
    for (auto &e : activeEntities) {
        e->FixedUpdate();
    }
}
