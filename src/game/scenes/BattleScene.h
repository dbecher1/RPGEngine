//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_BATTLESCENE_H
#define RPG_BATTLESCENE_H

#include <array>
#include "../battle/PartyMember.h"
#include "../battle/Enemy.h"
#include "../gfx/Environment.h"
#include "../ResourceManager.h"
#include "Scene.h"

struct BattleSceneBuilder {
    std::string env_name;
};

class BattleScene : public Scene {
public:
    explicit BattleScene(BattleSceneBuilder builder);
    void Update(double dt) override;
    void FixedUpdate() override;
    void Draw(SpriteBatch* sb) const override;
    void OnBattleEnd();
private:
    Environment background;
    std::array<PartyMember, 4> party;
    std::array<Enemy, 5> enemies;
};


#endif //RPG_BATTLESCENE_H
