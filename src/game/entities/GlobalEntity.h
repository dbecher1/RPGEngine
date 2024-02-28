//
// Created by Daniel Becher on 2/27/24.
//

#ifndef GLOBALENTITY_H
#define GLOBALENTITY_H

#include <optional>
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
    std::optional<std::string> defaultAnim{};
    bool is_player = false;
    ElementalAffinity affinity;
    Attributes stats;
    AnimationStateMachine overworldAnimations;
    AnimationStateMachine battleAnimations;
};

class GlobalEntity : public IBaseEntity {
public:
    GlobalEntity() = delete;
    explicit GlobalEntity(const EntityBuilder& eb);
    void Update(double dt) override;
    void Draw(DrawCommand *dc) override;;
private:
    std::string EntityName;
    ElementalAffinity Affinity;
    Attributes Stats;
    bool IsActive;
    // TODO: abilities and status
    BattleEntity BattleEntity;
    OverworldEntity OverworldEntity;
};



#endif //GLOBALENTITY_H
