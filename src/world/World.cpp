/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

const Point2u TEXTURE_SIZE(32);
const uint32_t ATLAS_DEPTH(1);

World::World():
    atlas { TEXTURE_SIZE, ATLAS_DEPTH },
    camera { Point3f(0.0f, 0.0f, 400.0f), Point3f(0.0f, 0.0f, 0.0f) },
    architect { },
    grid { 40, architect, atlas } {
    INFO("Creating world");
    LoadAtlas();
    grid.GivePositionUpdate(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::LoadAtlas() {
    assert(ATLAS_DEPTH == 1);
    std::vector<uint8_t> texData;
    BitmapResult bmp(ReadBitmap("data/atlas.bmp"));
    texData.reserve(TEXTURE_SIZE[0] * TEXTURE_SIZE[1] * 3);

    auto bmpIter = bmp.data.begin();
    for (uint32_t y = 0; y < TEXTURE_SIZE[1]; y++) {
        for (uint32_t x = 0; x < TEXTURE_SIZE[0]; x++) {
            auto lineStart = bmpIter;
            auto lineEnd = bmpIter + TEXTURE_SIZE[0] * 3;
            texData.insert(texData.end(),
                           lineStart,
                           lineEnd);
            bmpIter = lineEnd;
        }
        bmpIter += bmp.size[0] * 3;
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
