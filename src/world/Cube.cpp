/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Cube.hpp"

namespace mc::world {


Cube::Cube(const Point3f& position_, const Texture& texture_):
    Entity(position_, Point3f(0.0f, 0.0f, 0.0f)),
    texture { texture_ } {
}

void Cube::Draw(const Camera& camera, const Mesh& mesh) const {
    camera.LoadMVPMatrix(model);
    texture.MakeActive();
    mesh.Draw();
}



}   // namespace mc::world
