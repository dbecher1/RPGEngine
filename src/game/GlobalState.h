//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_GLOBALSTATE_H
#define RPG_GLOBALSTATE_H

#include "SDL_types.h"
#include "misc/macros.h"
#include "battle/Job.h"
#include <vector>
#include <map>

struct PartyMember_Data {
    std::string Name;
    Uint32 Level = 1;
    Uint64 Experience = 0;
    Uint32 Max_Health;
    Uint32 Max_Mana;
    Uint32 Attack;
    Uint32 Strength;
    Uint32 Defense;
    Uint32 Vitality;
    Uint32 Stagger;
    Uint32 Will;
    Uint32 Speed;
    Job Job;
    bool Is_Active;
};

struct GlobalState {
    double GameTime_Accumulator = 0;
    Uint8 GameTime_Seconds = 0;
    Uint8 GameTime_Minutes = 0;
    Uint64 GameTime_Hours = 0;

    Uint32 GameWindow_CurrentWidth = WINDOW_WIDTH;
    Uint32 GameWindow_CurrentHeight = WINDOW_HEIGHT;
    bool GameWindow_FullScreen = false;
    Uint32 GameWindow_CurrentResolution_Width = INITIAL_WIDTH;
    Uint32 GameWindow_CurrentResolution_Height = INITIAL_HEIGHT;

    PartyMember_Data Clive {
        .Name = "Clive",
        .Max_Health = 250,
        .Max_Mana = 50,
        .Attack = 15,
        .Strength = 15,
        .Defense = 10,
        .Vitality = 10,
        .Stagger = 10,
        .Will = 5,
        .Speed = 10,
        .Job = Knight,
        .Is_Active = true,
    };

    PartyMember_Data Jill {
            .Name = "Jill",
            .Max_Health = 200,
            .Max_Mana = 100,
            .Attack = 15,
            .Strength = 10,
            .Defense = 10,
            .Vitality = 7,
            .Stagger = 12,
            .Will = 15,
            .Speed = 13,
            .Job = Spellsword,
            .Is_Active = false,
    };

    static GlobalState* GetGlobalState();

    void GameTime_Tick(double dt);

    void SaveGameState(int slot);
    void LoadGameState(int slot);
};



#endif //RPG_GLOBALSTATE_H
