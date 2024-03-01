//
// Created by Daniel Becher on 2/27/24.
//

#ifndef OVERWORLDENTITY_H
#define OVERWORLDENTITY_H

#include "../gfx/animations/AnimationStateMachine.h"
#include "IBaseEntity.h"

struct EntityBuilder;

class OverworldEntity final : public IBaseEntity {
public:
    OverworldEntity() = delete;
    explicit OverworldEntity(const EntityBuilder& eb);
    void Update(double dt) override;
    void Draw(DrawCommand *dc) override;
    void FixedUpdate() override;

    Vector2 getPosition() const;
    Vector2 getDrawOffset();
private:
    AnimationStateMachine Animations;
    int z_layer;
    float moveSpeed;
    Vector2 position{};
    Vector2 delta{};
    bool IsPlayer;
    // TODO: I hate this
    std::string currState, lastState;
};

#endif //OVERWORLDENTITY_H
