/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

struct ChunkElement {
    Chunk&        chunk;
    ChunkElement* neighbour[4];
};

class ChunkGrid {

 public:
                    ChunkGrid(int32_t size);
                

 private:
     int32_t        size;
     ChunkElement   center;

};
