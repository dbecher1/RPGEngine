//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_JOB_H
#define RPG_JOB_H

// TODO: figure out what Vitality does lol

// I don't like using "job", I prefer class, but... can't do that lol
// Jobs essentially in my system just act as experience modifiers
struct Job {
    double Health_Mod;
    double Mana_Mod;
    double Attack_Mod;
    double Strength_Mod;
    double Defense_Mod;
    double Vitality_Mod;
    double Stagger_Mod;
    double Will_Mod;
    double Speed_Mod;
};

constexpr Job Knight {
        1.0,
        0.5,
        1.25,
        1.5,
        1.25,
        1.0,
        1.25,
        0.75,
        0.75
};

constexpr Job Spellsword {
        0.75,
        1.25,
        1.25,
        1.1,
        0.5,
        0.5,
        1.0,
        1.5,
        1.5
};

constexpr Job BlackMage {
    0.5,
    1.75,
    0.5,
    0.5,
    0.9,
    0.9,
    1.5,
    2.0,
    1.0
};

#endif //RPG_JOB_H
