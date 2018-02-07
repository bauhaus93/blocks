/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace mc {

//TODO maybe find more appropriate name? "Point" not good if used as Rotation
class Point {
 public:

                Point(float x_, float y_, float z_);
    virtual     ~Point() = default;

    float       GetX() const { return x; }
    float       GetY() const { return y; }
    float       GetZ() const { return z; }
    void        SetX(float value);
    void        SetY(float value);
    void        SetZ(float value);

    Point&      operator=(const Point& rhs);
    Point&      operator+=(const Point& rhs);

    const glm::mat4& GetMatrix() const;
    glm::vec3        CreateGLMVec() const;

protected:
    virtual void        UpdateMatrix() = 0;
    float      x;
    float      y;
    float      z;
    glm::mat4  matrix;
};


}   // namespace mc
