/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc {

World::World():
    camera { Position(4.0, 4.0, 0), Rotation(0, 0, 0) },
    mesh { "cube.obj" },
    texture { "test.bmp" },
    cube { Position(0, 0, 0), mesh, texture },
    cube2 { Position(2, 0, 0), mesh, texture } {
}

void World::Tick() {
    //cube.Move(Position(0, 0, 0.01));
    //cube.Rotate(Rotation(0, 0, 0.1));
    //cube2.Rotate(Rotation(0.1, -0.1, 0));
}

void World::Draw() const {
    cube.Draw(camera);
    cube2.Draw(camera);
}


}   // namespace mc
