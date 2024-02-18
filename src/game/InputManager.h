//
// Created by Daniel Becher on 2/17/24.
//

#ifndef RPG_INPUTMANAGER_H
#define RPG_INPUTMANAGER_H

#include "SDL_events.h"

// TODO: remapping

struct InputState {
    bool    Up = false,
            Down = false,
            Left = false,
            Right = false,
            Action = false,
            Pause = false;
};

class InputManager {
public:
    static void Poll(SDL_Event& e);
    static InputState* GetState();
private:
    static InputState State;
};


#endif //RPG_INPUTMANAGER_H
