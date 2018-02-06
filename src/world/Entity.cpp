/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Entity.hpp"

namespace mc {

//TODO currently ignoring model variable
Entity::Entity(glm::vec3 position_, glm::vec2 rotation_):
    position { position_ },
    rotation { rotation_ },
    model { glm::mat4(1.0f) } {
}

Entity::Entity(const Entity& other):
    position { other.position },
    rotation { other.rotation } {
}

Entity& Entity::operator=(const Entity& lhs) {
    position = lhs.position;
    rotation = lhs.rotation;
    return *this;
}

void Entity::SetPosition(glm::vec3 newPosition) {
    position = newPosition;
}
void Entity::SetRotation(glm::vec2 newRotation) {
    rotation = newRotation;
}

void Entity::Move(glm::vec3 offset) {
    position += offset;
}

void Entity::Rotate(glm::vec2 offset) {
    rotation += offset;
}

}   // namespace mc
