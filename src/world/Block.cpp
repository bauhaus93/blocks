/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Block.hpp"

namespace mc::world {


Block::Block(const Point3f& position_, const Texture& texture_, uint8_t neighbourCount_):
    Entity(position_, Point3f(0.0f, 0.0f, 0.0f)),
    texture { texture_ },
    neighbourCount { neighbourCount_ } {
}

Block::Block(const Block& other):
    Entity(other),
    texture { other.texture },
    neighbourCount { other.neighbourCount } {
}

void Block::Draw(const Camera& camera, const Mesh& mesh) const {
    camera.LoadMVPMatrix(model);
    texture.MakeActive();
    mesh.Draw();
}

void Block::IncreaseNeighbourCount(uint8_t amount) {
    neighbourCount += amount;
    assert(neighbourCount <= 6);
}

void Block::DecreaseNeighbourCount(uint8_t amount) {
    assert(amount <= neighbourCount);
    neighbourCount -= amount;
}


}   // namespace mc::world
