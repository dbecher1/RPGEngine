//
// Created by Daniel Becher on 2/16/24.
//

#include "Environment.h"

Environment::Environment(const EnvironmentBuilder &eb) :
name(eb.name), z(eb.z) {
    if (eb.use_spriteName) {
        useSpriteName = true;
        spriteName = eb.sprite_name;
    }
}

void Environment::Draw(DrawCommand *dc) {
    dc->SpriteName = useSpriteName ? spriteName.c_str() : name.c_str();
    dc->z = z;
    dc->staticSprite = true;
}