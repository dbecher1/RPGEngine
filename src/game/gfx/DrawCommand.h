//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_DRAWCOMMAND_H
#define RPG_DRAWCOMMAND_H

#include "../../math/Vector2.h"
#include "../../tools/Tile.h"

// TODO: expand upon descriptions for everything
// TODO: add in an origin offset
// TODO: see if I can consolidate bools into enums

struct DrawCommand {
    const char* SpriteName{};
    Vector2 position{0, 0};
    Vector2 dimensions{-1, -1};
    Vector2 offset{-1, -1}; // used in animations
    int z{0}; // MUST BE 0-9 (inclusive)
    bool useOffset = false;
    bool useDimensions = false;
    bool staticSprite = false;
    bool overrideSrcRect = false;
    Tile* tileOverride{};
    bool battle_scale = false; // this is janky but doing a quick and dirty fix right now
};


#endif //RPG_DRAWCOMMAND_H
