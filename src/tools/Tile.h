//
// Created by Daniel Becher on 2/20/24.
//

#ifndef RPG_TILE_H
#define RPG_TILE_H

#include "SDL_rect.h"

struct Tile {
    SDL_Rect src;
    SDL_FRect dest;
};

#endif //RPG_TILE_H
