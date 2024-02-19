//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_ENTITY_H
#define RPG_ENTITY_H

//#include "../SpriteBatch.h"
#include "../../math/Vector2.h"
#include "../gfx/animations/AnimationStateMachine.h"
#include "../InputManager.h" // TODO: move to player class

struct EntityBuilder {
    std::string name;
    AnimationStateMachine animationStateMachine;
    float move_speed;
    bool animated = false;
    bool use_eid = false; // for sub-animations
};

class SpriteBatch;

class Entity {
public:
    Entity() = delete;
    Entity(std::string name_, AnimationStateMachine animationSM, float move_speed);
    Entity(std::string name_, AnimationStateMachine animationSM, float move_speed, bool is_animated);
    Entity(EntityBuilder eb);
    void Update(double dt);
    void Draw(SpriteBatch* sb);
    void setDefaultAnimationState(const std::string& state);
    void setPlayer();
    int getId();
    static int EID;
protected:
    int id;
    bool isPlayer = false;
    std::string name;
    Vector2 position{};
    Vector2 delta{};
    AnimationStateMachine animationStateMachine;
    float moveSpeed;
    bool isAnimated = false;
};


#endif //RPG_ENTITY_H
