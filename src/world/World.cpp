/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

const Point2u TEXTURE_SIZE(32u);
const uint32_t ATLAS_DEPTH(2u);

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
    assert(ATLAS_DEPTH == 2);
    Image img = ReadBitmap("data/atlas.bmp");
    uint32_t mudLayer = 0;
    uint32_t grassLayer = 0;

    {   Image sub = img.CreateSubImage(Point2u(0u), TEXTURE_SIZE);
        mudLayer = atlas.AddTextureLayer(std::move(sub));
    }
    {
        Image sub = img.CreateSubImage(Point2u(32u, 0u), TEXTURE_SIZE);
        grassLayer = atlas.AddTextureLayer(std::move(sub));
    }

    protoblocks.emplace(BlockType::MUD, BlockType::MUD);
    for (uint8_t i = 0; i < 6; i++) {
        protoblocks.at(BlockType::MUD).AddFace(GetDirection(i), mudLayer);
    }

    protoblocks.emplace(BlockType::GRASS, BlockType::GRASS);
    for (uint8_t i = 0; i < 6; i++) {
        Direction dir = GetDirection(i);
        if (dir == Direction::UP) {
            protoblocks.at(BlockType::GRASS).AddFace(dir, grassLayer);
        } else {
            protoblocks.at(BlockType::GRASS).AddFace(dir, mudLayer);
        }
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
