//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_VECTOR2_H
#define RPG_VECTOR2_H

struct Vector2f;

struct Vector2 {
    int x, y;
    Vector2 operator + (const Vector2& rhs) const;
    void operator += (const Vector2& rhs);
    void operator += (const Vector2f& rhs);
};

struct Vector2f {
    float x, y;
    Vector2f operator * (float scalar) const;
    void operator *= (float scalar);
};



#endif //RPG_VECTOR2_H
