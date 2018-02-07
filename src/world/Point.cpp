/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Point.hpp"

namespace mc {

Point::Point(float x_, float y_, float z_):
    x { x_ },
    y { y_ },
    z { z_ } {
}

Point& Point::operator=(const Point& rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
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

const glm::mat4& Point::GetMatrix() const {
    return matrix;
}

glm::vec3 Point::CreateGLMVec() const {
    return glm::vec3(x, y, z);
}


}       // namespace mc
