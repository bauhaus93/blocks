/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Entity.hpp"

namespace mc::world {

Entity::Entity(Point3f position_, Point3f rotation_):
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

Entity& Entity::operator=(const Entity& rhs) {
    position = rhs.position;
    rotation = rhs.rotation;
    model = rhs.model;
    return *this;
}

void Entity::UpdateModel() {
    model = CreateTranslationMatrix(position) *
            CreateRotationMatrix(rotation) *
            glm::mat4(1.0f);
}

void Entity::Move(const Point3f& offset) {
    position += offset;
    UpdateModel();
}

void Entity::Rotate(const Point3f& offset) {
    constexpr float DOUBLE_PI = 2 * M_PI;
    rotation += offset;
    for (uint8_t i = 0; i < 3; i++) {
        if (rotation[i] < 0) {
            rotation[i] += DOUBLE_PI;
        } else if (rotation[i] > DOUBLE_PI) {
            rotation[i] -= DOUBLE_PI;
        }
    }
    UpdateModel();
}

}   // namespace mc::world
