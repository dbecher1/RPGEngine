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

Vector2 Vector2::operator*(float scalar) const {
    return {this->x * scalar, this->y * scalar};
}

void Vector2::operator*=(float scalar) {
    this->x *= scalar;
    this->y *= scalar;
}
