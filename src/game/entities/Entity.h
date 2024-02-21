//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_ENTITY_H
#define RPG_ENTITY_H

#include "../../math/Vector2.h"
#include "../gfx/animations/AnimationStateMachine.h"
#include "../InputManager.h" // TODO: move to player class
#include <optional>

struct EntityBuilder {
    std::string name;
    AnimationStateMachine animationStateMachine;
    float move_speed;
    bool animated = false;
    int z;
    std::optional<std::string> defaultAnim{};
    bool is_player = false;
};

class SpriteBatch;

class Entity {
public:
    Entity() = delete;
    explicit Entity(const EntityBuilder& eb);
    void Update(double dt);
    void Draw(SpriteBatch* sb);
    int getId();
    static int EID;
protected:
    int id;
    int z_layer;
    float moveSpeed;
    std::string name;
    Vector2 position{};
    Vector2 delta{};
    std::string currState;
    std::string lastState;
    AnimationStateMachine animationStateMachine;
    bool hasIdleAnimation = false;
    bool isAnimated = false;
    bool isPlayer = false;
};


#endif //RPG_ENTITY_H
