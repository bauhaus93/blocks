/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Position.hpp"
#include "Rotation.hpp"


namespace mc {

class Entity {

 public:

                    Entity(Position position_, Rotation rotation_);
                    Entity(const Entity& other);
    virtual         ~Entity() = default;

    Entity&         operator=(const Entity& lhs);

    void            Move(const Position& offset);
    virtual void    Rotate(const Rotation& offset);

 protected:

     void           UpdateModel();

     Position       position;
     Rotation       rotation;
     glm::mat4      model;

};



}   // namespace mc
