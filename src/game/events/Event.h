//
// Created by Daniel Becher on 2/27/24.
//

#ifndef EVENT_H
#define EVENT_H

#include <SDL_stdinc.h>
#include <variant>

namespace RPG {

    enum EventType {
        NONE = -1,
        BATTLE_START,
        BATTLE_END,
        PAUSED,
    };

    struct NoEvent{};

    struct BattleStart {
        Uint16 EntityGroupID;
        Uint16 EnvironmentID;
    };

    struct BattleEnd {
        Uint16 Idk;
    };

    struct Event {
        EventType type = NONE;
        std::variant<NoEvent, BattleStart, BattleEnd> data;
    };

}

#endif //EVENT_H
