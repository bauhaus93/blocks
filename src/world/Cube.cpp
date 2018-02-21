/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Cube.hpp"

namespace mc::world {


Cube::Cube(const Position& position_, const Texture& texture_):
    Entity(position_, Rotation(0, 0, 0)),
    texture { texture_ } {

}

void Cube::Draw(const Camera& camera, const Mesh& mesh) const {
    camera.LoadMVPMatrix(model);
    texture.MakeActive();
    mesh.Draw();
}



}   // namespace mc::world
