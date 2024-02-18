//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_SPRITEBATCH_H
#define RPG_SPRITEBATCH_H

#include "SDL.h"
#include "gfx/DrawCommand.h"
#include "ResourceManager.h"
#include <vector>
#include <array>
#include <thread>

/// Important note regarding rendering layers!!!
/// As of right now, the Z layers are strict and specific
/// 0, 1, 2 - Ground/below entity layer
/// 3, 4, 5 - Entity layer (includes buildings!)
/// 6, 7, 8 - Above entity layer
/// 9 - Maybe UI?
/// This is important because only the Entity layers will be Y-sorted

#define NUM_DRAW_LAYERS 10
#define NUM_THREADS 3

class SpriteBatch {
public:
    SpriteBatch(SDL_Renderer* renderer_, ResourceManager* resourceManager_);
    void Draw();
    void Add(DrawCommand drawCommand);
private:
    SDL_Renderer* renderer;
    SDL_Texture* atlas;
    ResourceManager* resourceManager;
    std::array<std::vector<DrawCommand>, NUM_DRAW_LAYERS> DrawCommands;

};


#endif //RPG_SPRITEBATCH_H
