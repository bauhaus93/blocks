/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "BlockManager.hpp"

namespace mc::world {

BlockManager::BlockManager(graphics::TextureAtlas& atlas) {
    LoadFaces(atlas);
    LoadProtoblocks();
}

void  BlockManager::LoadFaces(graphics::TextureAtlas& atlas) {
    constexpr unsigned FACE_COUNT = 5;
    static const std::array<const char*, FACE_COUNT> faceNames = { { 
        "mud", "grass", "grass-mud", "desert", "rock" } };
    Image img = ReadBitmap("data/atlas.bmp");
    Point2u imgSize = img.GetSize();
    Point2u textureSize = atlas.GetTextureSize();
    Point2u currPos(0);

    for (unsigned i = 0; i < FACE_COUNT; ++i) {
        Image sub = img.CreateSubImage(currPos, textureSize);
        faces[faceNames[i]] = atlas.AddTextureLayer(std::move(sub));
        
        currPos[0] += textureSize[0];
        if (currPos[0] > imgSize[0]) {
            currPos[0] = 0;
            currPos[1] += textureSize[1];
            if (currPos[1] > imgSize[1] && i + 1 < FACE_COUNT) {
                throw ApplicationError("UnexpectedImageSIze", __FUNCTION__, "Wanted to load additional face data, but at end of image");
            }
        }
    }
}

void BlockManager::LoadProtoblocks() {
    
    AddBlock(BlockType::MUD); 
    for (unsigned i = 0; i < 6; i++) {
        auto& protoMud = protoblocks.at(BlockType::MUD);
        protoMud.SetFace(GetDirection(i),
                         faces.at("mud"));
    }

    AddBlock(BlockType::GRASS); 
    for (unsigned i = 0; i < 6; i++) {
        auto& protoGrass = protoblocks.at(BlockType::GRASS);
        Direction dir = GetDirection(i);
        if (dir == Direction::UP) {
            protoGrass.SetFace(dir, faces.at("grass"));
        } else if (dir == Direction::DOWN) {
            protoGrass.SetFace(dir, faces.at("mud"));
        } else {
            protoGrass.SetFace(dir, faces.at("grass-mud"));
        }
    }
    
    AddBlock(BlockType::DESERT); 
    for (unsigned i = 0; i < 6; i++) {
        auto& protoDesert = protoblocks.at(BlockType::DESERT);
        protoDesert.SetFace(GetDirection(i),
                            faces.at("desert"));
    }
   
    AddBlock(BlockType::ROCK); 
    for (unsigned i = 0; i < 6; i++) {
        auto& protoRock = protoblocks.at(BlockType::ROCK);
        protoRock.SetFace(GetDirection(i),
                          faces.at("rock"));
    }
}

void BlockManager::AddBlock(BlockType type) {
    assert(protoblocks.find(type) == protoblocks.end());
    protoblocks.emplace(type, ProtoBlock());
}


void BlockManager::SetBlockFace(BlockType type, Direction dir, uint32_t atlasIndex) {
    assert(protoblocks.find(type) != protoblocks.end());
    protoblocks.at(type).SetFace(dir, atlasIndex);
}

uint32_t BlockManager::GetBlockFace(BlockType type, Direction dir) const {
    assert(protoblocks.find(type) != protoblocks.end());
    return protoblocks.at(type).GetFace(dir);
}

}       // namespace mc::world
