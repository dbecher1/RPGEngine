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
    switch (GlobalState::CurrentGlobalState) {
        case OVERWORLD: {
            OverworldEntity.Update(dt);
        }
            break;
        case BATTLE: {
            BattleEntity.Update(dt);
        }
            break;
        default: break;
    }
}

void GlobalEntity::Draw(DrawCommand *dc) {
    switch (GlobalState::CurrentGlobalState) {
        case OVERWORLD: {
            OverworldEntity.Draw(dc);
        }
        break;
        case BATTLE: {
            BattleEntity.Draw(dc);
        }
        break;
        default: break;
    }
}

void GlobalEntity::FixedUpdate() {
    switch (GlobalState::CurrentGlobalState) {
        case OVERWORLD: {
            OverworldEntity.FixedUpdate();
        }
        break;
        case BATTLE: {
            BattleEntity.FixedUpdate();
        }
        break;
        default: break;
    }
}
