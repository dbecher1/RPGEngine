//
// Created by Daniel Becher on 2/17/24.
//
#include "Entity.h"
#include "../SpriteBatch.h"
#include <iostream>
#include <utility>

Entity::Entity(std::string name_, AnimationStateMachine animationSM, int move_speed)
: name(std::move(name_)), animationStateMachine(std::move(animationSM)), moveSpeed(move_speed) {
    std::cout << name << ": Hi!" << std::endl;
}

Entity::Entity(std::string name_, AnimationStateMachine animationSM, int move_speed, bool is_animated)
        : Entity(std::move(name_), std::move(animationSM), move_speed) {
    isAnimated = is_animated;
}

void Entity::Update(double dt) {
    animationStateMachine.Update(dt);
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
    // TODO: normalize Vectors
    position += delta * static_cast<float>(dt);
}

void Entity::Draw(SpriteBatch *sb) {
    DrawCommand drawCommand;
    drawCommand.position = position;
    if (isAnimated)
        animationStateMachine.Draw(&drawCommand);
    sb->Add(std::move(drawCommand));
}



void Entity::setDefaultAnimationState(const std::string &state) {
    animationStateMachine.SetState(state);
}

void Entity::setPlayer() {
    isPlayer = true;
}


