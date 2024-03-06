//
// Created by Daniel Becher on 2/17/24.
//

#include "AnimationStateMachine.h"
#include <iostream>

AnimationStateMachine::AnimationStateMachine(std::string  name) : name(std::move(name)){}

void AnimationStateMachine::Draw(DrawCommand *dc) {
    dc->SpriteName = currentState.c_str();
    animationStates[currentState].Draw(dc);
}

void AnimationStateMachine::Update(const double dt) {
    // if (!is_playing) return;
    animationStates[currentState].Update(dt);
}

void AnimationStateMachine::SetState(const std::string &state_) {
    const std::string state = !name.empty() ? appendMap[state_] : state_;
    //if (state == currentState) return;
    if (!currentState.empty())
        animationStates[currentState].Reset();
    currentState = state;
    if (!is_playing) {
        is_playing = true;
        animationStates[currentState].forceFrameAdvance();
    }
}

void AnimationStateMachine::AddAnimation(const std::string &anim_name, Animation anim) {
    std::string newName = anim_name;
    newName.append("_").append(name);
    appendMap[anim_name] = newName;
    animationStates.emplace(newName, anim);
    if (currentState.empty())
        currentState = newName;
    // std::cout << "Successfully added animation " << newName << " to state machine" << std::endl;
}

/**
 * Stops the animation from playing.
 * Any call to setstate will restart the playing
 */
void AnimationStateMachine::Stop() {
    is_playing = false;
    animationStates[currentState].Reset();
}

bool AnimationStateMachine::isPlaying() const {
    return is_playing;
}

Vector2 AnimationStateMachine::getSize() const {
    return animationStates.at(currentState).getSize();
}
