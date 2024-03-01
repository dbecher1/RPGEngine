//
// Created by Daniel Becher on 2/27/24.
//

#include "EntityManager.h"
#include "entities/IBaseEntity.h"
#include "SpriteBatch.h"

EntityManager::~EntityManager() {
    for (auto& [_, e] : Entities) {
        delete e;
    }
}

// This method takes the assumption that heap allocation has already been performed for us
void EntityManager::AddEntity(const std::string &name, IBaseEntity *entity) {
    Entities[name] = entity;
}

void EntityManager::DestroyEntity(const std::string &name) {
    delete Entities[name];
    Entities.erase(name);
}

void EntityManager::ActivateEntity(const std::string &name) {
    Entities[name]->Activate();
}

void EntityManager::DeactivateEntity(const std::string &name) {
    Entities[name]->Deactivate();
}

void EntityManager::Update(double dt) {
    for (auto& [_, e] : Entities) {
        e->Update(dt);
    }
}

void EntityManager::Draw(SpriteBatch *sb) {
    for (const auto& [_, e] : Entities) {
        if (e->isActive()) {
            DrawCommand dc;
            e->Draw(&dc);
            sb->SubmitDraw();
        }
    }
}
