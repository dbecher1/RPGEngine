//
// Created by Daniel Becher on 2/15/24.
//

#ifndef RPG_SPRITE_H
#define RPG_SPRITE_H

#include <string>
#include <map>
#include "animations/AnimationStateMachine.h"
#include "DrawCommand.h"
#include "../../math/Vector2.h"

class Sprite {
public:
    Sprite() = delete;
    Sprite(const std::string& name, bool animated);
    ~Sprite();

    void Draw(DrawCommand* dc);
private:
    std::string name;
    bool isAnimated = false;
    AnimationStateMachine* animationStateMachine = nullptr;
};


#endif //RPG_SPRITE_H
