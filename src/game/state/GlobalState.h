//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_GLOBALSTATE_H
#define RPG_GLOBALSTATE_H

#include "../misc/macros.h"

enum CurrentState {
    ERR = -1,
    MENU,
    OVERWORLD,
    BATTLE,
    PAUSE,
    SHOP,
};

struct GlobalState {
    static double GameTime_Accumulator;
    static Uint8 GameTime_Seconds;
    static Uint8 GameTime_Minutes;
    static Uint64 GameTime_Hours;

    static Uint32 GameWindow_CurrentWidth;
    static Uint32 GameWindow_CurrentHeight;
    static bool GameWindow_FullScreen;
    static Uint32 GameWindow_CurrentResolution_Width;
    static Uint32 GameWindow_CurrentResolution_Height;

    static CurrentState CurrentGlobalState, PreviousGlobalState;

    static void GameTime_Tick(double dt);

    static void SaveGameState(int slot);
    static void LoadGameState(int slot);
};



#endif //RPG_GLOBALSTATE_H
