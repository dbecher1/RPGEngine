//
// Created by Daniel Becher on 2/17/24.
//

#include "AnimationStateMachine.h"
#include <iostream>

AnimationStateMachine::AnimationStateMachine(std::string  name) : name(std::move(name)){}

void AnimationStateMachine::Draw(DrawCommand *dc) const {
    dc->SpriteName = currentState.c_str();
    animationStates.at(currentState).Draw(dc);
}

void AnimationStateMachine::Update(const double dt) {
    if (!is_playing) return;
    animationStates.at(currentState).Update(dt);
}

void AnimationStateMachine::SetState(const std::string &state_) {
    if (!is_playing)
        is_playing = true;
    const std::string state = !name.empty() ? appendMap[state_] : state_;
    if (state == currentState) return;
    if (!currentState.empty())
        animationStates.at(currentState).Reset();
    currentState = state;
}

void AnimationStateMachine::AddAnimation(const std::string &anim_name, Animation anim) {
    std::string newName = anim_name;
    newName.append("_").append(name);
    appendMap.emplace(anim_name, newName);
    animationStates.emplace(newName, anim);
    if (currentState.empty())
        currentState = newName;
    std::cout << "Successfully added animation " << newName << " to state machine" << std::endl;
}

/**
 * Stops the animation from playing.
 * Any call to setstate will restart the playing
 */
void AnimationStateMachine::Stop() {
    is_playing = false;
    animationStates.at(currentState).Reset();
}

bool AnimationStateMachine::isPlaying() const {
    return is_playing;
}

Vector2 AnimationStateMachine::getSize() const {
    return animationStates.at(currentState).getSize();
}
