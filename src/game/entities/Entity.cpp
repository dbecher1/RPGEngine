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
        InputState inputState = *InputManager::GetState();
        if (inputState.Up)
            delta.y += -1;
        if (inputState.Down)
            delta.y += 1;
        if (inputState.Left)
            delta.x += -1;
        if (inputState.Right)
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
    position += delta * static_cast<float>(dt) * moveSpeed;
}

void Entity::Draw(SpriteBatch *sb) {
    DrawCommand drawCommand;
    drawCommand.position = position;
    drawCommand.z = z_layer;
    if (isAnimated)
        animationStateMachine.Draw(&drawCommand);
    sb->Add(drawCommand);
}

