//
// Created by Daniel Becher on 2/24/24.
//

#include "GlobalState.h"

double GlobalState::GameTime_Accumulator = 0;
Uint8 GlobalState::GameTime_Seconds = 0;
Uint8 GlobalState::GameTime_Minutes = 0;
Uint64 GlobalState::GameTime_Hours = 0;

Uint32 GlobalState::GameWindow_CurrentWidth = WINDOW_WIDTH;
Uint32 GlobalState::GameWindow_CurrentHeight = WINDOW_HEIGHT;
bool GlobalState::GameWindow_FullScreen = false;
Uint32 GlobalState::GameWindow_CurrentResolution_Width = INITIAL_WIDTH;
Uint32 GlobalState::GameWindow_CurrentResolution_Height = INITIAL_HEIGHT;

CurrentState GlobalState::CurrentGlobalState = ERR;
CurrentState GlobalState::PreviousGlobalState = ERR;

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

