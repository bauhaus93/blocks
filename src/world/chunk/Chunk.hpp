/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <cstdint>
#include <bitset>

#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Defs.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Mesh.hpp"
#include "world/Block.hpp"
#include "world/Camera.hpp"

#include "SimplexNoise.hpp"

namespace mc::world::chunk {

class Chunk {

 public:
     static constexpr int32_t   SIZE = 16;

     typedef std::bitset<Chunk::SIZE * Chunk::SIZE> SingleBorderMask;
     typedef std::array<SingleBorderMask, 6> BorderMask;

    explicit        Chunk(const Point3i& chunkPos_);
                    Chunk(Chunk&& other);

    void            Generate(const SimplexNoise& noise, const Texture& texture);
    const Point3i&  GetPosition() const { return chunkPos; }
    bool            IsEmpty() const;
    bool            IsFull() const;
    bool            BlockExists(const Point3i& blockPos) const;
    void            DrawBlocks(const Camera& camera, const Mesh& mesh) const;
 private:
    void    GenerateColumn(Point3i top, const Texture& texture, std::array<int32_t, 4>& neighbourHeight);
    void    CreateNonBorderRenderCandidates();


    const Point3i   chunkPos;
    const Point3f   origin;
    uint8_t         neighbourChecked;
    BorderMask      borderMask;
    Map3D<Block>    blocks;
    VecRef<Block>   renderCandidates;
};

typedef std::array<int32_t, Chunk::SIZE * Chunk::SIZE> HeightArray;

}       // namespace mc::world::chunk
