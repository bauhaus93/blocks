/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Entity.hpp"

namespace mc::world {

Entity::Entity():
    position(0.0f),
    rotation(0.0f, 0.0f, 1.0f),
    model { glm::mat4(1.0f) } {
    UpdateModel();
}

void Entity::UpdateModel() {
    model = CreateTranslationMatrix(position) *
            CreateRotationMatrix(rotation);
}

void Entity::SetPosition(const Point3f& newPosition) {
    position = newPosition;
    UpdateModel();
}
void Entity::SetRotation(const Point3f& newRotation) {
    rotation = newRotation;
    UpdateModel();  // TODO maybe set flag and only update when next time needed?
}

void Entity::Move(const Point3f& offset) {
    position += offset;
    UpdateModel();  // TODO maybe set flag and only update when next time needed?
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
