#include <cmath>

#include "gtest/gtest.h"

#include "utility/Point3.hpp"
#include "world/chunk/Blocktree.hpp"
#include "world/BlockType.hpp"

using namespace mc;
using namespace mc::world;

TEST(TestBlocktree, SingleBlockInsertionsCheckTopLevelOctantAndDepth) {
    std::array<Point3i8, 8> positions { {
        Point3i8(7, 7, 7),
        Point3i8(8, 7, 7),
        Point3i8(7, 8, 7),
        Point3i8(8, 8, 7),
        Point3i8(7, 7, 8),
        Point3i8(8, 7, 8),
        Point3i8(7, 8, 8),
        Point3i8(8, 8, 8)
    } };

    for (uint8_t i = 0; i < 8; i++) {
        chunk::Blocktree bt(Point3i(0), 16);
        std::vector<chunk::BlockElement> e;
        e.emplace_back(positions.at(i), BlockType::MUD);
        bt.InsertBlocks(e);
        ASSERT_TRUE(bt.HasChild(i));
        ASSERT_EQ(log2(CHUNK_SIZE), bt.MaxDepth());
    }
}

TEST(TestBlocktree, FullBlocktreeCheckFullMerge) {
    std::vector<chunk::BlockElement> elements;
    elements.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

    for (uint8_t i = 0; i < CHUNK_SIZE; i++) {
        for (uint8_t j = 0; j < CHUNK_SIZE; j++) {
            for (uint8_t k = 0; k < CHUNK_SIZE; k++) {
                elements.emplace_back(Point3i8(i, j, k), BlockType::MUD);
            }
        }
    }
    chunk::Blocktree bt(Point3i(0), 16);
    ASSERT_TRUE(bt.IsEmpty());
    bt.InsertBlocks(std::move(elements));
    ASSERT_FALSE(bt.IsEmpty());
    ASSERT_TRUE(bt.IsLeaf());
    ASSERT_EQ(BlockType::MUD, bt.GetBlockType());
    ASSERT_EQ(0, bt.MaxDepth());
}

TEST(TestBlocktree, FullBlocktree2DiffBlocksTopBotCheckMerge) {
    std::vector<chunk::BlockElement> elements;
    elements.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

    for (uint8_t i = 0; i < CHUNK_SIZE; i++) {
        for (uint8_t j = 0; j < CHUNK_SIZE; j++) {
            for (uint8_t k = 0; k < CHUNK_SIZE; k++) {
                if (k < CHUNK_SIZE / 2) {
                    elements.emplace_back(Point3i8(i, j, k), BlockType::MUD);
                } else {
                    elements.emplace_back(Point3i8(i, j, k), BlockType::GRASS);
                }
            }
        }
    }
    chunk::Blocktree bt(Point3i(0), 16);
    ASSERT_TRUE(bt.IsEmpty());
    bt.InsertBlocks(std::move(elements));
    ASSERT_FALSE(bt.IsEmpty());
    ASSERT_FALSE(bt.IsLeaf());
    ASSERT_EQ(1, bt.MaxDepth());
    for (uint8_t oct = 0; oct < 4; oct++) {
        auto& t = bt.GetChild(oct);
        ASSERT_TRUE(t.IsLeaf());
        ASSERT_EQ(BlockType::MUD, t.GetBlockType());
    }
    for (uint8_t oct = 4; oct < 8; oct++) {
        auto& t = bt.GetChild(oct);
        ASSERT_TRUE(t.IsLeaf());
        ASSERT_EQ(BlockType::GRASS, t.GetBlockType());
    }
}

TEST(TestBlocktree, Blocktree4DiffCols1EmptyCheckMerge) {
    std::vector<chunk::BlockElement> elements;
    elements.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

    for (uint8_t i = 0; i < CHUNK_SIZE; i++) {
        for (uint8_t j = 0; j < CHUNK_SIZE; j++) {
            for (uint8_t k = 0; k < CHUNK_SIZE; k++) {
                if (i < CHUNK_SIZE / 2) {
                    if (j < CHUNK_SIZE / 2) {
                        elements.emplace_back(Point3i8(i, j, k), BlockType::MUD);
                    } else {
                        elements.emplace_back(Point3i8(i, j, k), BlockType::GRASS);
                    }
                } else {
                    if (j < CHUNK_SIZE / 2) {
                        elements.emplace_back(Point3i8(i, j, k), BlockType::ROCK);
                    }
                }
            }
        }
    }
    chunk::Blocktree bt(Point3i(0), 16);
    bt.InsertBlocks(std::move(elements));
    ASSERT_FALSE(bt.IsEmpty());
    ASSERT_FALSE(bt.IsLeaf());
    ASSERT_EQ(1, bt.MaxDepth());

    {
        auto& tLower = bt.GetChild(0);
        ASSERT_TRUE(tLower.IsLeaf());
        ASSERT_EQ(BlockType::MUD, tLower.GetBlockType());

        auto& tHigher = bt.GetChild(4);
        ASSERT_TRUE(tHigher.IsLeaf());
        ASSERT_EQ(BlockType::MUD, tHigher.GetBlockType());
    }
    {
        auto& tLower = bt.GetChild(2);
        ASSERT_TRUE(tLower.IsLeaf());
        ASSERT_EQ(BlockType::GRASS, tLower.GetBlockType());

        auto& tHigher = bt.GetChild(6);
        ASSERT_TRUE(tHigher.IsLeaf());
        ASSERT_EQ(BlockType::GRASS, tHigher.GetBlockType());
    }
    {
        auto& tLower = bt.GetChild(1);
        ASSERT_TRUE(tLower.IsLeaf());
        ASSERT_EQ(BlockType::ROCK, tLower.GetBlockType());

        auto& tHigher = bt.GetChild(5);
        ASSERT_TRUE(tHigher.IsLeaf());
        ASSERT_EQ(BlockType::ROCK, tHigher.GetBlockType());
    }

    ASSERT_FALSE(bt.HasChild(3));
    ASSERT_FALSE(bt.HasChild(7));
}