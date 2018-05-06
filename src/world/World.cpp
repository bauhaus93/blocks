/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

const Point2u TEXTURE_SIZE = Point2u(32);
const uint32_t ATLAS_DEPTH(5);

static graphics::ShaderProgram LoadShader();

World::World():
    shader { LoadShader() },
    atlas { TEXTURE_SIZE, ATLAS_DEPTH },
    camera { shader },
    fog { shader },
    blockManager { atlas }, 
    architect { blockManager },
    grid { architect } {
    INFO("Creating world");

    SetDrawDistance(5);
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
