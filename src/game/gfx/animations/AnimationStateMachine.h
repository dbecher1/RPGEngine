//
// Created by Daniel Becher on 2/17/24.
//

#ifndef RPG_ANIMATIONSTATEMACHINE_H
#define RPG_ANIMATIONSTATEMACHINE_H

#include "Animation.h"
#include <string>
#include <map>

class AnimationStateMachine {
public:
    // TODO: create manifest layout for animation data to be stored
    // TODO: create AnimationStateMachine constructor
    void Update(double dt);
    // Do nothing on same state
    void SetState(const std::string& state);
    void Draw(DrawCommand* dc);

    // TODO this might be temporary
    void AddAnimation(const std::string& name, Animation anim);
private:
    std::string currentState;
    std::map<std::string, Animation> animationStates;
};


#endif //RPG_ANIMATIONSTATEMACHINE_H
