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
/*
inline ElementalAffinity getTypeFromString(const char* s) {
    switch (s) {
        case "Fire": return FIRE;
        case "Ice": return ICE;
        case "Thunder": return THUNDER;
        case "Earth": return EARTH;
        case "Water": return WATER;
        case "Wind": return WIND;
        case "Light": return LIGHT;
        case "Dark": return DARK;
        default: break;
    }
    return NONE;
}
*/

#endif //ELEMENTALAFFINITY_H
