//
// Created by Daniel Becher on 2/17/24.
//

#include "Animation.h"

Animation::Animation(const int framesX, const int framesY, const double speed, const Vector2 atlasSize)
: size({atlasSize.x / static_cast<float>(framesX), atlasSize.y / static_cast<float>(framesY)}), numFramesX(framesX), numFramesY(framesY), animationSpeed(speed * ANIMATION_SPEED_MULTIPLIER) {
    totalFrames = numFramesX * numFramesY;
}

void Animation::Update(const double dt) {
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
    dc->offset.y = static_cast<float>(currentFrame / numFramesX); // NOLINT(*-integer-division)
    dc->dimensions = size;
}

// Experimental method, seeing what happens if I force a frame as soon as an animation wakes up
void Animation::forceFrameAdvance() {
    currentFrame++;
}

void Animation::Reset() {
    currentFrame = 0;
    accumulator = 0;
}

Vector2 Animation::getSize() const {
    return size;
}


