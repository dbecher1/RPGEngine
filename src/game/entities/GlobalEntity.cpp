//
// Created by Daniel Becher on 2/27/24.
//

#include "GlobalEntity.h"
#include "../state/GlobalState.h"

GlobalEntity::GlobalEntity(const EntityBuilder& eb) :
EntityName(eb.name), Affinity(eb.affinity),
Stats(eb.stats), IsActive(true),
BattleEntity(eb), OverworldEntity(eb) {}

void GlobalEntity::Update(double dt) {

}

void GlobalEntity::Draw(DrawCommand *dc) {
}

