/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ProtoBlock.hpp"

namespace blocks {

ProtoBlock::ProtoBlock():
    atlasIndices { { 0, 0, 0, 0, 0, 0 } } {
}

void ProtoBlock::SetFace(Direction dir, uint32_t atlasIndex) {
    assert(GetIndex(dir) < 6);
    atlasIndices[GetIndex(dir)] = atlasIndex;
}

uint32_t ProtoBlock::GetFace(Direction dir) const {
    assert(GetIndex(dir) < 6);
    return atlasIndices[GetIndex(dir)];
}


}   // namespace blocks
