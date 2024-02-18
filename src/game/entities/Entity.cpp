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

void Entity::Update(double dt) {
    animationStateMachine.Update(dt);
    // other things here
}

void Entity::Draw(SpriteBatch *sb) {
    DrawCommand drawCommand;
    drawCommand.position = position;
    if (isAnimated)
        animationStateMachine.Draw(&drawCommand);
    sb->Add(std::move(drawCommand));
}

Entity::Entity(std::string name_, AnimationStateMachine animationSM, int move_speed, bool is_animated)
: Entity(name_, animationSM, move_speed) {
    isAnimated = is_animated;
}


