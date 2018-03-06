/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "utility/PointTransformation.hpp"

namespace mc::world {

class Entity {

 public:

                    Entity(Point3f position_, Point3f rotation_);
                    Entity(const Entity& other);
    virtual         ~Entity() = default;

    Entity&         operator=(const Entity& rhs);

    virtual void    Move(const Point3f& offset);
    virtual void    Rotate(const Point3f& offset);

    const Point3f&  GetPosition() const { return position; }
    const Point3f& GetRotation() const { return rotation; }

 protected:

     void           UpdateModel();

     Point3f        position;
     Point3f        rotation;
     glm::mat4      model;

};



}   // namespace mc::world
