/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Object.hpp"

namespace mc {

Object::Object(glm::vec3 position_, glm::vec2 rotation_):
    position { position_ },
    rotation { rotation_ } {
}

Object::Object(const Object& other):
    position { other.position },
    rotation { other.rotation } {

}

void Object::SetPosition(glm::vec3 newPosition) {
    position = newPosition;
}
void Object::SetRotation(glm::vec2 newRotation) {
    rotation = newRotation;
}

void Object::Move(glm::vec3 offset) {
    position += offset;
}

void Object::Rotate(glm::vec2 offset) {
    rotation += offset;
}

}   // namespace mc
