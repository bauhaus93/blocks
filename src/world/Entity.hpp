/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace mc {

//TODO handle model variable
class Entity {

 public:

                    Entity(glm::vec3 position_, glm::vec2 rotation_);
                    Entity(const Entity& other);
    Entity&         operator=(const Entity& lhs);

    virtual         ~Entity() = default;

    void            SetPosition(glm::vec3 newPosition);
    void            SetRotation(glm::vec2 newRotation);

    virtual void    Rotate(glm::vec2 offset);
    void            Move(glm::vec3 offset);

 protected:

    glm::vec3      position;
    glm::vec2      rotation;
    glm::mat4      model;

};



}   // namespace mc
