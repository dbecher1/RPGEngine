//
// Created by Daniel Becher on 2/17/24.
//

#include "InputManager.h"
#include <iostream>

InputState InputManager::State{};

void InputManager::Poll(SDL_Event &e) {
    // TODO: Eventually add controller support...
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.scancode) {
            case SDL_SCANCODE_W: case SDL_SCANCODE_UP: {
                if (!State.Up)
                    State.Up = true;
                break;
            }
            case SDL_SCANCODE_A: case SDL_SCANCODE_LEFT: {
                if (!State.Left)
                    State.Left = true;
                break;
            }
            case SDL_SCANCODE_S: case SDL_SCANCODE_DOWN: {
                if (!State.Down)
                    State.Down = true;
                break;
            }
            case SDL_SCANCODE_D: case SDL_SCANCODE_RIGHT: {
                if (!State.Right)
                    State.Right = true;
                break;
            }
            default: break;
        }
    }
    else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.scancode) {
            case SDL_SCANCODE_W: case SDL_SCANCODE_UP: {
                if (State.Up)
                    State.Up = false;
                break;
            }
            case SDL_SCANCODE_A: case SDL_SCANCODE_LEFT: {
                if (State.Left)
                    State.Left = false;
                break;
            }
            case SDL_SCANCODE_S: case SDL_SCANCODE_DOWN: {
                if (State.Down)
                    State.Down = false;
                break;
            }
            case SDL_SCANCODE_D: case SDL_SCANCODE_RIGHT: {
                if (State.Right)
                    State.Right = false;
                break;
            }
            default: break;
        }
    }
}

InputState *InputManager::GetState() {
    return &State;
}
