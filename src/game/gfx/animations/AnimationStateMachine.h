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
    explicit AnimationStateMachine(std::string  name);

    void Update(double dt);
    void SetState(const std::string& state_);
    void Draw(DrawCommand* dc) const;
    void Stop();

    void AddAnimation(const std::string& anim_name, Animation anim);

    [[nodiscard]] bool isPlaying() const;
    [[nodiscard]] Vector2 getSize() const;

private:
    std::string name; // used for uniquely identifying the entity
    std::string currentState{};
    std::map<std::string, Animation> animationStates;
    // More to store in memory, but will make our access time faster by caching the appends
    std::map<std::string, std::string> appendMap;
    bool is_playing = true;
};


#endif //RPG_ANIMATIONSTATEMACHINE_H
