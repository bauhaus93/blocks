/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include <gtest/gtest.h>

#include "utility/Point3.hpp"
#include "world/architect/GlobalPosition.hpp"

namespace mc::tests {

TEST(TestGlobalPosition, NegativeBlockPosShouldBeSameGlobalPos) {
    Point3i chunkPos(0, 0, 0);
    Point3i nbChunkPos(-1, 0, 0);
    Point3i8 blockPos(-1, 0, 0);
    Point3i8 nbBlockPos(mc::world::CHUNK_SIZE - 1, 0, 0);

    ASSERT_EQ(world::architect::GetGlobalPosition(chunkPos, blockPos),
              world::architect::GetGlobalPosition(nbChunkPos, nbBlockPos));
}

TEST(TestGlobalPosition, ChunkPlusBlockPosShouldBeSameGlobalPos) {
    Point3i chunkPos(0, 0, 0);
    Point3i nbChunkPos(1, 0, 0);
    Point3i8 blockPos(mc::world::CHUNK_SIZE, 0, 0);
    Point3i8 nbBlockPos(0, 0, 0);

    ASSERT_EQ(world::architect::GetGlobalPosition(chunkPos, blockPos),
              world::architect::GetGlobalPosition(nbChunkPos, nbBlockPos));
}

}
