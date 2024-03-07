//
// Created by Daniel Becher on 2/27/24.
//

#include "BattleEntity.h"
#include "GlobalEntity.h"

BattleEntity::BattleEntity(const EntityBuilder &eb) :
Animations(eb.battleAnimations) {}

void BattleEntity::Draw(DrawCommand *dc) {
    dc->z = 5;
    Animations.Draw(dc);
}

void BattleEntity::FixedUpdate() {
}

void BattleEntity::Update(double dt) {
}
