//
// Created by Daniel Becher on 2/7/24.
//

#include "OverworldScene.h"
#include "../ResourceManager.h"
#include "../SpriteBatch.h"

OverworldScene::OverworldScene(const SceneBuilder& sb, ResourceManager* rm) {
    sceneName = sb.name;
    if (!sb.is_active)
        isActive = false;
    for (const auto& s : sb.entities) {
        activeEntities.push_back(rm->getEntity(s));
    }
    for (const auto& ptr : sb.entities_by_ptr) {
        activeEntities.push_back(ptr);
    }
    map = rm->getMap(sb.map_name);
}

void OverworldScene::Update(double dt) {
    for (auto &e : activeEntities) {
        e->Update(dt);
    }
}

void OverworldScene::AddEntity(const std::string &name) {
    // TODO
}

void OverworldScene::Draw(SpriteBatch *sb) const {
    map->Draw(sb);
    for (const auto &e : activeEntities) {
        DrawCommand dc;
        e->Draw(&dc);
        sb->Add(dc);
    }
}

void OverworldScene::FixedUpdate() {
    for (auto &e : activeEntities) {
        e->FixedUpdate();
    }
}

SDL_Point OverworldScene::getCurrentWorldBoundaries() const {
    return map->getDimensions();
}
