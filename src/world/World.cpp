/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

const Point2u TEXTURE_SIZE(32u);
const uint32_t ATLAS_DEPTH(1u);

World::World():
    atlas { TEXTURE_SIZE, ATLAS_DEPTH },
    protoblocks { },
    camera { Point3f(0.0f, 0.0f, 400.0f), Point3f(0.0f, 0.0f, 0.0f) },
    architect { protoblocks },
    grid { 40, architect } {
    INFO("Creating world");
    LoadProtoBlocks();
    grid.GivePositionUpdate(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::LoadProtoBlocks() {
    assert(ATLAS_DEPTH == 1);
    Image img = ReadBitmap("data/atlas.bmp");

    Image sub = img.CreateSubImage(Point2u(0u), Point2u(32u));
    uint32_t layer = atlas.AddTextureLayer(std::move(sub));
    auto result = protoblocks.emplace(BlockType::MUD, BlockType::MUD);
    for (uint8_t i = 0; i < 6; i++) {
        result.first->second.AddFace(GetDirection(i), layer);
    }
}

void World::Tick() {
    grid.GivePositionUpdate(camera.GetPosition());
    grid.Update();
}

void World::Draw() const {
    grid.Draw(camera);
}

}   // namespace mc::world
