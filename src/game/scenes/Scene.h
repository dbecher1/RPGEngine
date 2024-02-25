//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include "../SpriteBatch.h"

class Scene {
public:
    virtual ~Scene() = default;
    virtual void Update(double dt) = 0;
    virtual void FixedUpdate() = 0;
    virtual void Draw(SpriteBatch* sb) const = 0;
protected:
    std::string sceneName;
};

#endif //RPG_SCENE_H
