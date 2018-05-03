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

                    Entity();
                    Entity(const Entity& other) = default;
    virtual         ~Entity() = default;

    Entity&         operator=(const Entity& rhs) = default;

    virtual void    SetPosition(const Point3f& newPosition);
    virtual void    SetRotation(const Point3f& newRotation);
    virtual void    Move(const Point3f& offset);
    virtual void    Rotate(const Point3f& offset);

    const Point3f&  GetPosition() const { return position; }
    const Point3f&  GetRotation() const { return rotation; }

 protected:

     void           UpdateModel();

     Point3f        position;
     Point3f        rotation;
     glm::mat4      model;

};



}   // namespace mc::world
