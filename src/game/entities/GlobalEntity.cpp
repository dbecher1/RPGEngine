//
// Created by Daniel Becher on 2/27/24.
//

#include "GlobalEntity.h"
#include "../state/GlobalState.h"

GlobalEntity::GlobalEntity(const EntityBuilder& eb) :
EntityName(eb.name), Affinity(eb.affinity),
Stats(eb.stats),
BattleEntity(eb), OverworldEntity(eb) {
    active = eb.is_active;
}

void GlobalEntity::Update(double dt) {
    OverworldEntity.Update(dt);
}

void GlobalEntity::Draw(DrawCommand *dc) {
    OverworldEntity.Draw(dc);
}

void GlobalEntity::FixedUpdate() {
    OverworldEntity.FixedUpdate();
}
