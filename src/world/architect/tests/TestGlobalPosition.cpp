/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include <gtest/gtest.h>

#include "utility/Point3.hpp"
#include "world/architect/GlobalPosition.hpp"

namespace blocks::tests {

TEST(TestGlobalPosition, NegativeBlockPosShouldBeSameGlobalPos) {
    Point3i chunkPos(0, 0, 0);
    Point3i nbChunkPos(-1, 0, 0);
    Point3i8 blockPos(-1, 0, 0);
    Point3i8 nbBlockPos(blocks::CHUNK_SIZE - 1, 0, 0);

    ASSERT_EQ(GetGlobalPosition(chunkPos, blockPos),
              GetGlobalPosition(nbChunkPos, nbBlockPos));
}

TEST(TestGlobalPosition, ChunkPlusBlockPosShouldBeSameGlobalPos) {
    Point3i chunkPos(0, 0, 0);
    Point3i nbChunkPos(1, 0, 0);
    Point3i8 blockPos(blocks::CHUNK_SIZE, 0, 0);
    Point3i8 nbBlockPos(0, 0, 0);

    ASSERT_EQ(GetGlobalPosition(chunkPos, blockPos),
              GetGlobalPosition(nbChunkPos, nbBlockPos));
}

}
