/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ProtoBlock.hpp"

namespace mc::world::chunk {

ProtoBlock::ProtoBlock(BlockType type_):
    type { type_ } {
}

void ProtoBlock::AddFace(Direction dir, uint32_t atlasIndex) {
    assert(atlasLayer.find(dir) == atlasLayer.end());
    atlasLayer.emplace(dir, atlasIndex);
}

uint32_t ProtoBlock::GetFace(Direction face) const {
    assert(atlasLayer.find(face) != atlasLayer.end());
    return atlasLayer.at(face);
}


}   // namespace mc::world::chunk
