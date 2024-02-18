//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_ENTITY_H
#define RPG_ENTITY_H

//#include "../SpriteBatch.h"
#include "../../math/Vector2.h"
#include "../gfx/animations/AnimationStateMachine.h"

class SpriteBatch;

class Entity {
public:
    Entity() = delete;
    Entity(std::string name_, AnimationStateMachine animationSM, int move_speed);
    Entity(std::string name_, AnimationStateMachine animationSM, int move_speed, bool is_animated);
    void Update(double dt);
    void Draw(SpriteBatch* sb);

protected:
    std::string name;
    Vector2 position{};
    AnimationStateMachine animationStateMachine;
    int moveSpeed;
    bool isAnimated = false;
};


#endif //RPG_ENTITY_H
