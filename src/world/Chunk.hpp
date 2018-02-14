/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <map>
#include <functional>
#include <cstdint>

#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Mesh.hpp"

#include "Position.hpp"
#include "Cube.hpp"
#include "Camera.hpp"
#include "SimplexNoise.hpp"

namespace mc {

//typedef std::vector<std::reference_wrapper<T>> VecRef<T>;

class Chunk {
 public:
            Chunk(const Point2<int32_t>& chunkPos_,
                  const Point3<float>& blockSize_,
                  const Point2<int32_t>& gridSize_);

    void    Generate(const SimplexNoise& noise, const Mesh& mesh, const Texture& texture);
    void    Draw(const Camera& camera) const;
 private:
    void    GenerateColumn(Point3<int32_t> top, const Mesh& mesh, const Texture& texture);
    void    CreateRenderCandidates();

    const Point2<int32_t>               chunkPos;
    const Point3<float>                 blockSize;
    const Point2<int32_t>               gridSize;
    const Position                      origin;
    std::map<Point3<int32_t>, Cube>     grid;
    std::vector<std::reference_wrapper<Cube>>   renderCandidates;
};

}       // namespace mc
