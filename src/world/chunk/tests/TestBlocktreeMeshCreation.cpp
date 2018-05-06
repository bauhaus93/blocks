/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "TestBlocktreeMeshCreation.hpp"

using namespace mc;
using namespace mc::world;
/*
void TestBlocktreeMeshCreation::SetUp() {
    std::array<BlockType, 2> types { { BlockType::MUD, BlockType::GRASS } };
    for (uint8_t i = 0; i < 2; i++) {
        protoblocks.emplace(types[i], types[i]);
        for (uint8_t j = 0; j < 6; j++) {
            protoblocks.at(types[i]).AddFace(GetDirection(j), i);
        }
    }
    bt = chunk::Blocktree(Point3i8(0), CHUNK_SIZE);
}

TEST_F(TestBlocktreeMeshCreation, SingleBlockCheckFaces) {
    std::vector<chunk::BlockElement> blocks;
    blocks.emplace_back(Point3i8(1), BlockType::MUD);

    bt.InsertBlocks(std::move(blocks));

    mesh::Mesh mesh = bt.CreateMesh(protoblocks);
    ASSERT_EQ(6 * 2, mesh.GetTriangleCount());
}

TEST_F(TestBlocktreeMeshCreation, FullBlocks1TypeCheckFaces) {
    std::vector<chunk::BlockElement> blocks;

    for (uint8_t i = 0; i < CHUNK_SIZE; i++) {
        for (uint8_t j = 0; j < CHUNK_SIZE; j++) {
            for (uint8_t k = 0; k < CHUNK_SIZE; k++) {
                blocks.emplace_back(Point3i8(i, j, k), BlockType::MUD);
            }
        }
    }

    bt.InsertBlocks(std::move(blocks));

    mesh::Mesh mesh = bt.CreateMesh(protoblocks);
    ASSERT_EQ(6 * 2, mesh.GetTriangleCount());
}*/
