//
// Created by Daniel Becher on 2/24/24.
//

#include "BattleScene.h"
#include "../GlobalState.h"

BattleScene::BattleScene(BattleSceneBuilder builder) {
    // TODO: create enemy groups
    sceneName = "battle";
    auto state = GlobalState::GetGlobalState();

    int i = 0;

    if (state->Clive.Is_Active) {
        party[i++] = {state->Clive};
    }
    if (state->Jill.Is_Active) {
        party[i++] = {state->Jill};
    }
}

void BattleScene::Update(double dt) {

}

void BattleScene::Draw(SpriteBatch *sb) const {

}

void BattleScene::OnBattleEnd() {

}

void BattleScene::FixedUpdate() {

}
