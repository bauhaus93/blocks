/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Entity.hpp"

namespace mc {

Entity::Entity(Position position_, Rotation rotation_):
    position { position_ },
    rotation { rotation_ },
    model { glm::mat4(1.0f) } {
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
    model = glm::mat4(1.0f);    //TODO apply translation & rotation
}

void Entity::Move(const Position& offset) {
    position.Move(offset);
}

void Entity::Rotate(const Rotation& offset) {
    rotation.Rotate(offset);
}

}   // namespace mc
