//
// Created by Daniel Becher on 2/17/24.
//

#ifndef RPG_ANIMATION_H
#define RPG_ANIMATION_H

#include "../DrawCommand.h"
#include "../../../math/Vector2.h"

// I prefer to store my speed as ints (as ms)
#define ANIMATION_SPEED_MULTIPLIER 0.001

class Animation {
public:
    //Animation() = delete;
    Animation() = default;
    Animation(int framesX,int framesY, double speed, Vector2 atlasSize);
    void Update(double dt);
    void Draw(DrawCommand* dc) const;
    void forceFrameAdvance();
    void Reset();
    [[nodiscard]] Vector2 getSize() const;
private:
    Vector2 size;
    int currentFrame = 0, numFramesX, numFramesY, totalFrames;
    double accumulator = 0.0, animationSpeed;
};


#endif //RPG_ANIMATION_H
