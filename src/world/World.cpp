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
    std::vector<uint8_t> texData;
    BitmapResult bmp(ReadBitmap("data/atlas.bmp"));
    texData.reserve(TEXTURE_SIZE[0] * TEXTURE_SIZE[1] * 3);

    auto bmpIter = bmp.data.begin();
    for (uint32_t line = 0; line < TEXTURE_SIZE[1]; line++) {
        auto lineStart = bmpIter;
        auto lineEnd = bmpIter + TEXTURE_SIZE[0] * 3;
        texData.insert(texData.end(),
                        lineStart,
                        lineEnd);
        bmpIter = lineEnd;
        bmpIter += bmp.size[0] * 3;
    }
    uint32_t layer = atlas.AddTextureLayer(texData);
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
