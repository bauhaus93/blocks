/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Block.hpp"

namespace mc::world {


Block::Block(const Point3f& position_, const Texture& texture_):
    Entity(position_, Point3f(0.0f, 0.0f, 0.0f)),
    texture { texture_ } {
}

void Block::Draw(const Camera& camera, const Mesh& mesh) const {
    camera.LoadMVPMatrix(model);
    texture.MakeActive();
    mesh.Draw();
}



}   // namespace mc::world
