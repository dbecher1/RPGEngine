//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_ENTITY_H
#define RPG_ENTITY_H

//#include "../SpriteBatch.h"
#include "../../math/Vector2.h"
#include "../gfx/animations/AnimationStateMachine.h"
#include "../InputManager.h" // TODO: move to player class

class SpriteBatch;

class Entity {
public:
    Entity() = delete;
    Entity(std::string name_, AnimationStateMachine animationSM, int move_speed);
    Entity(std::string name_, AnimationStateMachine animationSM, int move_speed, bool is_animated);
    void Update(double dt);
    void Draw(SpriteBatch* sb);
    void setDefaultAnimationState(const std::string& state);
    void setPlayer();
protected:
    bool isPlayer = false;
    std::string name;
    Vector2 position{};
    Vector2f delta{};
    AnimationStateMachine animationStateMachine;
    float moveSpeed;
    bool isAnimated = false;
};


#endif //RPG_ENTITY_H
