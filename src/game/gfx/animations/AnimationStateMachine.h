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
    void SetState(const std::string& state_);
    void Draw(DrawCommand* dc) const;
    void Stop();
    bool isPlaying();

    // TODO this might be temporary
    void AddAnimation(const std::string& name, Animation anim);
    void AddAnimation(const std::string& name, Animation anim, int eid_);
private:
    int eid = -1;
    std::string currentState;
    std::map<std::string, Animation> animationStates;
    // More to store in memory, but will make our access time faster by caching the appends
    std::map<std::string, std::string> eidAppends;
    bool is_playing;
};


#endif //RPG_ANIMATIONSTATEMACHINE_H
