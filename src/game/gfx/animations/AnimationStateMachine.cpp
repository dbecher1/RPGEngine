//
// Created by Daniel Becher on 2/17/24.
//

#include "AnimationStateMachine.h"
#include <iostream>

void AnimationStateMachine::Draw(DrawCommand *dc) {
    dc->SpriteName = currentState.c_str();
    animationStates.at(currentState).Draw(dc);
}

void AnimationStateMachine::Update(double dt) {
    if (!is_playing) return;
    animationStates.at(currentState).Update(dt);
}

void AnimationStateMachine::SetState(const std::string &state_) {
    if (!is_playing)
        is_playing = true;
    // If we're using the eid, get that value from the appends map
    std::string state = (eid != -1) ? eidAppends[state_] : state_;
    if (state == currentState) return;
    if (!currentState.empty())
        animationStates.at(currentState).Reset();
    currentState = state;
}

void AnimationStateMachine::AddAnimation(const std::string &name, Animation anim) {
    animationStates.emplace(name, anim);
    if (currentState.empty())
        currentState = name;
    std::cout << "Successfully added animation " << name << " to state machine" << std::endl;
}

// This overload is important for entities with sub-animations
void AnimationStateMachine::AddAnimation(const std::string &name, Animation anim, int eid_) {
    if (eid != eid_)
        eid = eid_;
    std::string newName = name;
    newName.append(std::to_string(eid));
    eidAppends.emplace(name, newName);
    AddAnimation(newName, anim);
}

/**
 * Stops the animation from playing.
 * Any call to setstate will restart the playing
 */
void AnimationStateMachine::Stop() {
    is_playing = false;
    animationStates.at(currentState).Reset();
}
