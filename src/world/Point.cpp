/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Point.hpp"


Point::Point(float x, float y, float z):
    x { x_ },
    y { y_ },
    z { z_ } {
}

Point& Point::operator=(const Point& rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}

Point Point::operator+(const Point& rhs) const {
    return Point(x + rhs.x,
                 y + rhs.y,
                 z + rhs.z);
}
Point& Point::operator+=(const Point& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

void Point::SetX(float value) {
    x = value;
    UpdateMatrix();
}
void Point::SetY(float value) {
    y = value;
    UpdateMatrix();
}
void Point::SetZ(float value) {
    z = value;
    UpdateMatrix();
}

const glm::mat4& Position::GetMatrix() const {
    return matrix;
}
