/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Position.hpp"
#include "Rotation.hpp"


namespace mc::world {

class Entity {

 public:

                    Entity(Position position_, Rotation rotation_);
                    Entity(const Entity& other);
    virtual         ~Entity() = default;

    Entity&         operator=(const Entity& lhs);

    virtual void    Move(const Position& offset);
    virtual void    Rotate(const Rotation& offset);

    const Position& GetPosition() const { return position; }
    const Rotation& GetRotation() const { return rotation; }

 protected:

     void           UpdateModel();

     Position       position;
     Rotation       rotation;
     glm::mat4      model;

};



}   // namespace mc::world
