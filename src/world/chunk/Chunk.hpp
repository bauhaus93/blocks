/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <map>
#include <functional>
#include <memory>
#include <cstdint>

#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Mesh.hpp"
#include "world/Block.hpp"
#include "world/Camera.hpp"

#include "SimplexNoise.hpp"

namespace mc::world::chunk {

template<typename T>
using VecRef = std::vector<std::reference_wrapper<T>>;

template<typename T>
using Map3D = std::map<Point3i, T>;

template<typename T>
using MapRef3D = std::map<Point3i, std::reference_wrapper<T>>;

class Chunk {
 public:
            Chunk(const Point3i& chunkPos_,
                  const Point3i& chunkSize_,
                  const Point3f& blockSize_);
            Chunk(Chunk&& other);

    void            Generate(const SimplexNoise& noise, const Texture& texture);
    const Point3i&  GetPosition() const { return chunkPos; }
    bool            IsEmpty() const;
    bool            BlockExists(const Point3i& blockPos) const;
    void            DrawBlocks(const Camera& camera, const Mesh& mesh) const;
    void            CreateBorderRenderCandidates(const Map3D<Chunk>& chunks);
 private:
    void    GenerateColumn(Point3i top, const Texture& texture);
    void    CreateNonBorderRenderCandidates();


    const Point3i   chunkPos;
    const Point3i   chunkSize;
    const Point3f   blockSize;
    const Point3f   origin;
    Map3D<Block>     blocks;
    VecRef<Block>    renderCandidates;
    VecRef<Block>    renderCandidatesBorder;
};

}       // namespace mc::world::chunk
