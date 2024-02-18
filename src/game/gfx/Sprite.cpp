//
// Created by Daniel Becher on 2/15/24.
//

#include "Sprite.h"

Sprite::Sprite(const std::string &name, bool animated) {

}

Sprite::~Sprite() {

}

/// The way this flows is a little tricky. Sprites don't store a representation of their position, entities do.
/// Entities will receive the call to draw, instantiate the DrawCommand, and pass it along to their Sprite before adding it to the batch.
void Sprite::Draw(DrawCommand *dc) {
    dc->SpriteName = name.c_str();
    if (isAnimated) {
        animationStateMachine->Draw(dc);
    }
}
