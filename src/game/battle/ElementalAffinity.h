//
// Created by Daniel Becher on 2/27/24.
//

#ifndef ELEMENTALAFFINITY_H
#define ELEMENTALAFFINITY_H

enum ElementalAffinity {
    NONE = -1,
    FIRE,
    ICE,
    THUNDER,
    WATER,
    EARTH,
    WIND,
    LIGHT,
    DARK
};

inline ElementalAffinity getTypeFromString(const std::string& s) {
    ElementalAffinity e = NONE;
    if (s == "Fire")
        e = FIRE;
    else if (s == "Ice")
        e = ICE;
    else if (s == "Thunder")
        e = THUNDER;
    else if (s == "Earth")
        e = EARTH;
    else if (s == "Water")
        e = WATER;
    else if (s == "Wind")
        e = WIND;
    else if (s == "Light")
        e = LIGHT;
    else if (s == "Dark")
        e = DARK;
    return e;
}

#endif //ELEMENTALAFFINITY_H
