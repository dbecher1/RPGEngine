//
// Created by Daniel Becher on 2/27/24.
//

#include "BattleEntity.h"

#include "GlobalEntity.h"

BattleEntity::BattleEntity(const EntityBuilder &eb) :
Animations(eb.battleAnimations) {}

void BattleEntity::Draw(DrawCommand *dc) {
}

void BattleEntity::FixedUpdate() {
}

void BattleEntity::Update(double dt) {
}
