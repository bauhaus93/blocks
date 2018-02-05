/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace mc {

class Object {

 public:

                    Object(glm::vec3 position_, glm::vec2 rotation_);
                    Object(const Object& other);
    virtual         ~Object() = default;

    void            SetPosition(glm::vec3 newPosition);
    void            SetRotation(glm::vec2 newRotation);

    virtual void    Rotate(glm::vec2 offset);
    void            Move(glm::vec3 offset);

 private:

     glm::vec3      position;
     glm::vec2      rotation;

};



}   // namespace mc
