//
// Created by Daniel Becher on 2/17/24.
//

#include "Animation.h"

Animation::Animation(int framesX, int framesY, double speed, Vector2 atlasSize)
: numFramesX(framesX), numFramesY(framesY), animationSpeed(speed * ANIMATION_SPEED_MULTIPLIER), size({atlasSize.x / static_cast<float>(framesX), atlasSize.y / static_cast<float>(framesY)}) {
    totalFrames = numFramesX * numFramesY;
}

void Animation::Update(double dt) {
    accumulator += dt;
    if (accumulator > animationSpeed) {
        accumulator = 0;
        currentFrame++;
        if (currentFrame >= totalFrames) {
            currentFrame = 0;
        }
    }
}

void Animation::Draw(DrawCommand *dc) const {
    dc->useOffset = true;
    dc->useDimensions = true;
    dc->offset.x = static_cast<float>(currentFrame % numFramesX);
    dc->offset.y = static_cast<float>(static_cast<int>(currentFrame / numFramesX)); // ugly
    dc->dimensions = size;
}

void Animation::Reset() {
    currentFrame = 0;
    accumulator = 0;
}


