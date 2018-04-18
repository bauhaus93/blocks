/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

const Point2u TEXTURE_SIZE(32u);
const uint32_t ATLAS_DEPTH(5u);

World::World():
    atlas { TEXTURE_SIZE, ATLAS_DEPTH },
    protoblocks { },
    camera { Point3f(0.0f, 0.0f, 400.0f), Point3f(0.0f, 0.0f, 0.0f) },
    architect { protoblocks },
    grid { architect } {
    INFO("Creating world");
    LoadProtoBlocks();

    SetDrawDistance(15);
    grid.GivePositionUpdate(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::SetDrawDistance(int32_t chunkDrawDistance) {
    grid.SetDrawDistance(chunkDrawDistance);
    camera.SetFar((chunkDrawDistance - chunkDrawDistance / 10 ) * CHUNK_SIZE * BLOCK_SIZE);
}

void World::LoadProtoBlocks() {
    Image img = ReadBitmap("data/atlas.bmp");
    uint32_t mudLayer = 0;
    uint32_t grassLayer = 0;
    uint32_t grassMudLayer = 0;
    uint32_t desertLayer = 0;
    uint32_t rockLayer = 0;

    {   Image sub = img.CreateSubImage(Point2u(0u), TEXTURE_SIZE);
        mudLayer = atlas.AddTextureLayer(std::move(sub));
    }
    {
        Image sub = img.CreateSubImage(Point2u(32u, 0u), TEXTURE_SIZE);
        grassLayer = atlas.AddTextureLayer(std::move(sub));
    }
    {
        Image sub = img.CreateSubImage(Point2u(64u, 0u), TEXTURE_SIZE);
        grassMudLayer = atlas.AddTextureLayer(std::move(sub));
    }
    {
        Image sub = img.CreateSubImage(Point2u(96u, 0u), TEXTURE_SIZE);
        desertLayer = atlas.AddTextureLayer(std::move(sub));
    }
    {
        Image sub = img.CreateSubImage(Point2u(128u, 0u), TEXTURE_SIZE);
        rockLayer = atlas.AddTextureLayer(std::move(sub));
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
            protoblocks.at(BlockType::GRASS).AddFace(dir, grassMudLayer);
        }
     }

    protoblocks.emplace(BlockType::DESERT, BlockType::DESERT);
    for (uint8_t i = 0; i < 6; i++) {
        protoblocks.at(BlockType::DESERT).AddFace(GetDirection(i), desertLayer);
    }

    protoblocks.emplace(BlockType::ROCK, BlockType::ROCK);
    for (uint8_t i = 0; i < 6; i++) {
        protoblocks.at(BlockType::ROCK).AddFace(GetDirection(i), rockLayer);
    }
}

void World::Tick() {
    grid.GivePositionUpdate(camera.GetPosition());
    grid.Update();
}

void World::Draw() {
    camera.ActivateShader();
    atlas.Activate();
    grid.Draw(camera);
    atlas.Deactivate();
    camera.DeactivateShader();
}

}   // namespace mc::world
