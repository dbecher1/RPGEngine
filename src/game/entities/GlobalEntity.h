//
// Created by Daniel Becher on 2/27/24.
//

#ifndef GLOBALENTITY_H
#define GLOBALENTITY_H

#include <string>
#include "BattleEntity.h"
#include "OverworldEntity.h"
#include "../battle/Attributes.h"
#include "../gfx/animations/AnimationStateMachine.h"
#include "../battle/ElementalAffinity.h"

struct EntityBuilder {
    std::string name;
    float overworld_move_speed;
    bool animated = true;
    int z;
    bool is_active;
    AnimationStateMachine overworldAnimations;
    AnimationStateMachine battleAnimations;
    ElementalAffinity affinity;
    Attributes stats;
    bool is_player = false;
};

class GlobalEntity final : public IBaseEntity {
public:
    explicit GlobalEntity(const EntityBuilder& eb);
    void Update(double dt) override;
    void Draw(DrawCommand *dc) override;
    void FixedUpdate() override;
private:
    std::string EntityName;
    ElementalAffinity Affinity;
    Attributes Stats;
    // TODO: abilities and status
    BattleEntity BattleEntity;
    OverworldEntity OverworldEntity;
};



#endif //GLOBALENTITY_H
