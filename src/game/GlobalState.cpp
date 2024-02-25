//
// Created by Daniel Becher on 2/24/24.
//

#include "GlobalState.h"

static GlobalState state{};

GlobalState *GlobalState::GetGlobalState() {
    return &state;
}

void GlobalState::GameTime_Tick(double dt) {
    GameTime_Accumulator += dt;
    if (GameTime_Accumulator >= 1) {
        GameTime_Accumulator = 0;
        GameTime_Seconds++;
        if (GameTime_Seconds >= 60) {
            GameTime_Seconds = 0;
            GameTime_Minutes++;
            if (GameTime_Minutes >= 60) {
                GameTime_Minutes = 0;
                GameTime_Hours++;
            }
        }
    }
}

// TODO: serialization methods once
void GlobalState::SaveGameState(int slot) {

}

void GlobalState::LoadGameState(int slot) {

}

