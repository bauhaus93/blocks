/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

namespace mc {

//TODO maybe find more appropriate name? "Point" not good if used as Rotation
class Point {
 public:

                Point(float x, float y, float z);
    virtual     ~Point() = default;

    float       GetX() const { return x; }
    float       GetY() const { return y; }
    float       GetZ() const { return z; }
    void        SetX(float value);
    void        SetY(float value);
    void        SetZ(float value);

    Point&      operator=(const Point& rhs);
    Point       operator+(const Point& rhs) const;
    Point&      operator+=(const Point& rhs);

    const glm::mat4& Position::GetMatrix() const;

protected:
    void        UpdateMatrix() = 0;
    float      x;
    float      y;
    float      z;
    glm::mat4  matrix;
};


}   // namespace mc
