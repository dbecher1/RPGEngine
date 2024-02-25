//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_ENVIRONMENT_H
#define RPG_ENVIRONMENT_H

#include <string>
#include "DrawCommand.h"

struct EnvironmentBuilder {
    std::string name;
    // for when the sprite name != wanted display name
    std::string sprite_name;
    int z = 0;
    bool use_spriteName = false;
};

/**
 * Container class for the background displayed in a battle
 */
class Environment {
public:
    //Environment() = delete;
    Environment() = default;
    explicit Environment(const EnvironmentBuilder& eb);
    void Draw(DrawCommand* dc);
private:
    std::string name;
    std::string spriteName;
    int z;
    bool useSpriteName = false;
};


#endif //RPG_ENVIRONMENT_H
