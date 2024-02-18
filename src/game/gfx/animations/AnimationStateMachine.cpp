//
// Created by Daniel Becher on 2/17/24.
//

#include "AnimationStateMachine.h"
#include <iostream>

void AnimationStateMachine::Draw(DrawCommand *dc) {
    animationStates.at(currentState).Draw(dc);
}

void AnimationStateMachine::Update(double dt) {
    animationStates.at(currentState).Update(dt);
}

void AnimationStateMachine::SetState(const std::string &state) {
    if (state == currentState) return;
    animationStates.at(currentState).Reset();
    currentState = state;
}

void AnimationStateMachine::AddAnimation(const std::string &name, Animation anim) {
    animationStates.emplace(name, anim);
    if (currentState.empty())
        currentState = name;
    std::cout << "Successfully added animation " << name << " to state machine";
}
