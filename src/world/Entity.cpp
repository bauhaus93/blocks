/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Entity.hpp"

namespace mc {

Entity::Entity(Position position_, Rotation Rotation_):
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

void Entity::UpdateModel() {
    model = glm::mat4(1.0f);
}

void Entity::SetPosition(const Position& newPosition) {
    position = newPosition;
}

void Entity::SetRotation(const Rotation& newRotation) {
    rotation = newRotation;
}

void Entity::Move(const Position& offset) {
    position += offset;
}

void Entity::Rotate(const Rotation& offset) {
    rotation += offset;
}

}   // namespace mc
