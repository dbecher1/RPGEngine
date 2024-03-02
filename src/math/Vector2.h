//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_VECTOR2_H
#define RPG_VECTOR2_H

#include <cmath>

struct Vector2 {
    float x, y;
    [[nodiscard]] inline float Length() const {
        return std::sqrt((x * x) + (y * y));
    }
    inline void Normalize() {
        if (const float len = Length(); len > 0.0f) {
            x = x / len;
            y = y / len;
        }
    };
    inline void TryNormalize() {
        if (const float len = Length(); len > 1.0f) {
            x = x / len;
            y = y / len;
        }
    }
    [[nodiscard]] inline bool isZero() const {
        return (x == 0.0f) && (y == 0.0f);
    }
    inline Vector2 operator + (const Vector2& rhs) const {
        return {this->x + rhs.x, this->y + rhs.y};
    }
    inline void operator += (const Vector2& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
    }
    inline Vector2 operator * (const float scalar) const {
        return {this->x * scalar, this->y * scalar};
    }
    inline void operator *= (const float scalar) {
        this->x *= scalar;
        this->y *= scalar;
    }
};




#endif //RPG_VECTOR2_H
