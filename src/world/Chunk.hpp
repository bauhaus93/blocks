/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <map>

namespace mc {


struct BlockPos {
    int x;
    int y;
    int z;
};

class Chunk {
 public:
            Chunk(sizeX, sizeY, sizeZ);


 private:
    
    std::map<BlockPos, Cube>    blocks;
    //std::vector<Cube>           renderBlocks;



};


}       // namespace mc




