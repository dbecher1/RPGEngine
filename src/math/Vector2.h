//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_VECTOR2_H
#define RPG_VECTOR2_H

struct Vector2f;

struct Vector2 {
    float x, y;
    Vector2 operator + (const Vector2& rhs) const;
    void operator += (const Vector2& rhs);
    Vector2 operator * (float scalar) const;
    void operator *= (float scalar);
};




#endif //RPG_VECTOR2_H
