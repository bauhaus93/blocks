/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc {

World::World():
    camera { Position(4.0, 4.0, 4.0), Rotation(0, 0, 0) },
    mesh { "cube.obj" },
    texture { "test.bmp" },
    cube { Position(0, 0, 0), mesh, texture } {
}

void World::Draw() const {
    cube.Draw(camera);
}


}   // namespace mc
