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
};



#endif //BATTLEENTITY_H
