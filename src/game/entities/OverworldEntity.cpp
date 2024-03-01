//
// Created by Daniel Becher on 2/27/24.
//

#include "OverworldEntity.h"
#include "GlobalEntity.h"
#include "../InputManager.h"
#include "../misc/macros.h"

OverworldEntity::OverworldEntity(const EntityBuilder &eb) :
Animations(eb.overworldAnimations), z_layer(eb.z),
moveSpeed(eb.overworld_move_speed), IsPlayer(eb.is_player) {}

void OverworldEntity::Update(double dt) {
    delta = {0.0f, 0.0f};
    if (IsPlayer) {
        auto [Up, Down, Left, Right, Action, Pause] = *InputManager::GetState();
        if (Up)
            delta.y += -1;
        if (Down)
            delta.y += 1;
        if (Left)
            delta.x += -1;
        if (Right)
            delta.x += 1;
    }
    // TODO: make this state machine more robust
    // Particularly, make the animation names more.. concrete
    lastState = currState;
    if (delta.y == 0.0f) {
        if (delta.x > 0.0f) {
            currState = "WalkRight";
        }
        else if (delta.x < 0.0f) {
            currState = "WalkLeft";
        }
    }
    else if (delta.y > 0) {
        currState = "WalkDown";
    }
    else if (delta.y < 0) {
        currState = "WalkUp";
    }
    if (lastState != currState)
        Animations.SetState(currState);

    // TODO: this is only for sprites that have no idle animation... check that
    if (!delta.isZero()) {
        Animations.Update(dt);
    } else {
        Animations.Stop();
    }
    delta.TryNormalize();
}

void OverworldEntity::Draw(DrawCommand *dc) {
    dc->position = position;
    dc->z = z_layer;
    Animations.Draw(dc);
}

void OverworldEntity::FixedUpdate() {
    position += delta * moveSpeed * FIXED_UPDATE_INTERVAL;
}

Vector2 OverworldEntity::getPosition() const {
    return position;
}

Vector2 OverworldEntity::getDrawOffset() {
    return position + (Animations.getSize() * 0.5f);
}
