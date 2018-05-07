/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

static graphics::ShaderProgram LoadShader();

World::World():
    shader { LoadShader() },
    atlas { Point2u(32), 16 },
    camera { shader },
    fog { shader },
    blockManager { atlas },
    architect { blockManager },
    grid { architect } {
    INFO("Creating world");

    SetDrawDistance(40);
    grid.GivePositionUpdate(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::SetDrawDistance(int32_t chunkDrawDistance) {
    grid.SetDrawDistance(chunkDrawDistance);
    camera.SetFar((chunkDrawDistance - chunkDrawDistance / 10 ) * CHUNK_SIZE * BLOCK_SIZE);
}

void World::Tick() {
    grid.GivePositionUpdate(camera.GetPosition());
    grid.Update();
}

void World::Draw() {
    shader.Activate();
    atlas.Activate();
    grid.Draw(camera);
    atlas.Deactivate();
    shader.Deactivate();
}

static graphics::ShaderProgram LoadShader() {
    graphics::ShaderProgram program;
    program.AddVertexShader("shader/VertexShader.glsl");
    program.AddFragmentShader("shader/FragmentShader.glsl");
    program.Link();
    return program;
}

}   // namespace mc::world
