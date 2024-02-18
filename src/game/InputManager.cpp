//
// Created by Daniel Becher on 2/17/24.
//

#include "InputManager.h"
#include "SDL_keyboard.h"
#include <iostream>

InputState InputManager::State{};
const Uint8* InputManager::KeyboardState = nullptr;


void InputManager::Init() {
    KeyboardState = SDL_GetKeyboardState(nullptr);
}

void InputManager::Poll() {
    State.Up = KeyboardState[UP_] || KeyboardState[W_];
    State.Down = KeyboardState[DOWN_] || KeyboardState[S_];
    State.Left = KeyboardState[LEFT_] || KeyboardState[A_];
    State.Right = KeyboardState[RIGHT_] || KeyboardState[D_];
}

InputState *InputManager::GetState() {
    return &State;
}
