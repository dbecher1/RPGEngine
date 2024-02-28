//
// Created by Daniel Becher on 2/17/24.
//
#include "Entity.h"
#include "../SpriteBatch.h"
#include <iostream>
#include <utility>

int Entity::EID = 0;

Entity::Entity(const EntityBuilder& eb) :
    name(eb.name), animationStateMachine(eb.animationStateMachine),
    moveSpeed(eb.move_speed), isAnimated(eb.animated),
    z_layer(eb.z), isPlayer(eb.is_player) {
    if (eb.defaultAnim.has_value())
        animationStateMachine.SetState(eb.defaultAnim.value());
    id = EID++;
}


void Entity::Update(double dt) {
    // other things here
    delta = {0.0f, 0.0f};
    if (isPlayer) {
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
        animationStateMachine.SetState(currState);

    // TODO: this is only for sprites that have no idle animation... check that
    if (!delta.isZero()) {
        animationStateMachine.Update(dt);
    } else {
        animationStateMachine.Stop();
    }
    delta.TryNormalize();
}

void Entity::FixedUpdate() {
    position += delta * moveSpeed * FIXED_UPDATE_INTERVAL;
}

void Entity::Draw(DrawCommand *dc) const {
    dc->position = position;
    dc->z = z_layer;
    if (isAnimated)
        animationStateMachine.Draw(dc);
}

Vector2 Entity::getPosition() {
    return position;
}
// Similar to getposition except takes the size into account
Vector2 Entity::getDrawOffset() {
    return position + (animationStateMachine.getSize() * 0.5f);
}



