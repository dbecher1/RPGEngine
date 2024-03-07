//
// Created by Daniel Becher on 3/6/24.
//

#ifndef BATTLESCENE_H
#define BATTLESCENE_H
#include "../entities/GlobalEntity.h"
#include <array>

#include "Scene.h"
#include "../gfx/Environment.h"

// TODO: expand upon this
struct EnemyEnum {
    std::vector<std::string> enemies;
};

struct BattleBuilder {
    std::array<GlobalEntity*, 4> party{};
    int party_size = 0;
    EnemyEnum enemyGroup;
    Environment background;
};

class BattleScene final : public Scene {
public:
    BattleScene() = default;
    explicit BattleScene(const BattleBuilder& bb);
    void Update(double dt) override;
    void FixedUpdate() override;
    void Draw(SpriteBatch* sb) const override;
private:
    std::array<GlobalEntity*, 4> party{};
    float x_start{}, x_delta{}, y_start{}, y_delta{};
    int partySize{};
    EnemyEnum enemies;
    Environment bg;
};



#endif //BATTLESCENE_H
