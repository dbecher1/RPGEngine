//
// Created by Daniel Becher on 2/27/24.
//

#ifndef BATTLEENTITY_H
#define BATTLEENTITY_H

#include "IBaseEntity.h"
#include "../gfx/animations/AnimationStateMachine.h"

struct EntityBuilder;

class BattleEntity final : public IBaseEntity {
public:
    BattleEntity() = delete;
    explicit BattleEntity(const EntityBuilder& eb);
    void Update(double dt) override;
    void Draw(DrawCommand *dc) override;
    void FixedUpdate() override;
private:
    AnimationStateMachine Animations;
    // The ultimate position of the entity is going to be determined by the battle scene, this will just dictate the offset from that which they're drawn
    Vector2 positionOffset;
    bool frontLine = true; // emulating FFVI front/backline
};



#endif //BATTLEENTITY_H
