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
#include "world/Position.hpp"
#include "world/Cube.hpp"
#include "world/Camera.hpp"

#include "SimplexNoise.hpp"

namespace mc::world::chunk {

template<typename T>
using VecRef = std::vector<std::reference_wrapper<T>>;

template<typename T>
using Map3D = std::map<Point3i, T>;

class Chunk {
 public:
            Chunk(const Point2i& chunkPos_,
                  const Point2i& chunkSize_,
                  const Point3f& blockSize_);
            Chunk(Chunk&& other);

    void            Generate(const SimplexNoise& noise, const Mesh& mesh, const Texture& texture);
    const Point2i&  GetPosition() const { return chunkPos; }
    void            Draw(const Camera& camera) const;
 private:
    void    GenerateColumn(Point3i top, const Mesh& mesh, const Texture& texture);
    void    CreateRenderCandidates();

    const Point2i   chunkPos;
    const Point2i   chunkSize;
    const Point3f   blockSize;
    const Position  origin;
    Map3D<Cube>     blocks;
    VecRef<Cube>    renderCandidates;
};

}       // namespace mc::world::chunk
