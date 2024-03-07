//
// Created by Daniel Becher on 3/6/24.
//

#include "BattleScene.h"

BattleScene::BattleScene(const BattleBuilder& bb) :
party(bb.party), partySize(bb.party_size), enemies(bb.enemyGroup), bg(bb.background) {
    // TODO: remove this once enemy groups are actually implemented
    enemies.enemies.emplace_back("Morbol");

    // for every party member there is a shift vertically downwards of 0.0917
    // horizontal shift of 0.0278
    // "battle box" ranges 0.308-0.579 vertical
    // 0.69 (nice) - 0.775 horizontal
    constexpr float x_min = 0.69f;
    constexpr float x_max = 0.775f;
    constexpr float y_min = 0.308f;
    constexpr float y_max = 0.579f;

    // Put the party members in their appropriate slots depending on their position
    switch (partySize) {
        default: break;
        case 1: {
            x_start = (x_max + x_min) / 2.0f;
            y_start = (y_max + y_min) / 2.0f;
            break;
        }
        case 2: {
            x_delta = (x_max + x_min) / 4.0f;
            x_start = x_min + x_delta;
            x_delta *= 2;

            y_delta = (y_max + y_min) / 4.0f;
            y_start = y_min + y_delta;
            break;
        }
        case 3: {
            x_delta = (x_max + x_min) / 3.0f;
            x_start = x_min + (x_delta * 0.5f);

            y_delta = (y_max + y_min) / 3.0f;
            y_start = y_min + (y_delta * 0.5f);
            break;
        }
        case 4: {
            x_delta = (x_max + x_min) / 4.0f;
            x_start = x_min;
            y_delta = (y_max + y_min) / 4.0f;
            y_start = y_min;
        }
    }
}

void BattleScene::Update(double dt) {
    for (int i = 0; i < partySize; i++) {
        party[i]->Update(dt);
    }
}

void BattleScene::FixedUpdate() {
    for (int i = 0; i < partySize; i++) {
        party[i]->FixedUpdate();
    }
}

void BattleScene::Draw(SpriteBatch *sb) const {
    DrawCommand dc1;
    bg.Draw(&dc1);
    sb->Add(dc1);

    for (int i = 0; i < partySize; i++) {
        DrawCommand dc;
        dc.battle_scale = true;
        dc.position = {x_start + (x_delta * i), y_start + (y_delta * i)};
        party[i]->Draw(&dc);
        sb->Add(dc);
    }

    for (const auto& e: enemies.enemies) {
        DrawCommand dc;
        dc.battle_scale = true;
        dc.SpriteName = e.c_str();
        dc.position = {0.15f, 0.3f};
        sb->Add(dc);
    }
}
