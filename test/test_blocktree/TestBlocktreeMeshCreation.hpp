/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include <map>
#include <cmath>
#include <algorithm>
#include <random>

#include "gtest/gtest.h"

#include "utility/Point3.hpp"
#include "world/chunk/Blocktree.hpp"
#include "world/BlockType.hpp"
#include "world/ProtoBlock.hpp"
#include "world/Direction.hpp"
#include "world/Size.hpp"
#include "mesh/Mesh.hpp"


class TestBlocktreeMeshCreation: public ::testing::Test {
protected:
    virtual void SetUp();

    std::map<mc::world::BlockType, mc::world::ProtoBlock> protoblocks;
    mc::world::chunk::Blocktree bt;
};
