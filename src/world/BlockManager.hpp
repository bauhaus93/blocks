/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <map>
#include <cassert>

#include "utility/Bitmap.hpp"
#include "utility/Image.hpp"
#include "graphics/TextureAtlas.hpp"
#include "BlockType.hpp"
#include "ProtoBlock.hpp"

namespace mc::world {

class BlockManager {
 public:
                BlockManager(graphics::TextureAtlas& atlas);

    void        SetBlockFace(BlockType type, Direction dir, uint32_t atlasIndex);
    uint32_t    GetBlockFace(BlockType type, Direction dir) const;
 private:
    std::map<std::string, uint32_t>     faces;
    std::map<BlockType, ProtoBlock>     protoblocks;


    void         LoadFaces(graphics::TextureAtlas& atlas);
    void         LoadProtoblocks();
    void         AddBlock(BlockType type);
};


}       // namespace mc::world
