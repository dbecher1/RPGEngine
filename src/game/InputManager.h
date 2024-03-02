//
// Created by Daniel Becher on 2/17/24.
//

#ifndef RPG_INPUTMANAGER_H
#define RPG_INPUTMANAGER_H

#include "SDL_keyboard.h"

#define UP_ SDL_SCANCODE_UP
#define DOWN_ SDL_SCANCODE_DOWN
#define LEFT_ SDL_SCANCODE_LEFT
#define RIGHT_ SDL_SCANCODE_RIGHT
#define W_ SDL_SCANCODE_W
#define A_ SDL_SCANCODE_A
#define S_ SDL_SCANCODE_S
#define D_ SDL_SCANCODE_D

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
    static void Init();
    static void Poll();
    static InputState* GetState();
private:
    static InputState State;
    static const Uint8* KeyboardState; // pointer to the SDL array
};


#endif //RPG_INPUTMANAGER_H
