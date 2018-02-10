/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Entity.hpp"

namespace mc {

Entity::Entity(Position position_, Rotation rotation_):
    position { position_ },
    rotation { rotation_ },
    model { glm::mat4(1.0f) } {
    UpdateModel();
}

Entity::Entity(const Entity& other):
    position { other.position },
    rotation { other.rotation },
    model { other.model } {
}

Entity& Entity::operator=(const Entity& lhs) {
    position = lhs.position;
    rotation = lhs.rotation;
    model = lhs.model;
    return *this;
}

void Entity::UpdateModel() {
    model = position.CreateMatrix() * rotation.CreateMatrix()  * glm::mat4(1.0f);
}

void Entity::Move(const Position& offset) {
    position.Move(offset);
    UpdateModel();
}

void Entity::Rotate(const Rotation& offset) {
    rotation.Rotate(offset);
    UpdateModel();
}

}   // namespace mc
