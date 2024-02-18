//
// Created by Daniel Becher on 2/16/24.
//

#include "Vector2.h"
#include <cmath>

Vector2 Vector2::operator+(const Vector2 &rhs) const {
    return {this->x + rhs.x, this->y + rhs.y};
}

void Vector2::operator+=(const Vector2 &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
}

void Vector2::operator+=(const Vector2f &rhs) {
    this->x += static_cast<int>(std::lround(rhs.x));
    this->y += static_cast<int>(std::lround(rhs.y));
}

Vector2f Vector2f::operator*(float scalar) const {
    return {this->x * scalar, this->y * scalar};
}

void Vector2f::operator*=(float scalar) {
    this->x *= scalar;
    this->y *= scalar;
}
