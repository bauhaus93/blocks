/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Cube.hpp"

namespace mc {


Cube::Cube(const Position& position_, const Mesh& mesh_, const Texture& texture_):
    Entity(position_, Rotation(0, 0, 0)),
    mesh { mesh_ },
    texture { texture_ } {

}

void Cube::Draw(const Camera& camera) const {
    camera.LoadMVPMatrix(model);
    texture.MakeActive();
    mesh.Draw();
}



}   // namespace mc
