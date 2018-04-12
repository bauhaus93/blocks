/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>

#include "logger/GlobalLogger.hpp"
#include "utility/Defs.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "world/Direction.hpp"
#include "world/Size.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Quad.hpp"
#include "mesh/Triangle.hpp"
#include "Block.hpp"


namespace mc::world::chunk {

std::unique_ptr<mesh::Mesh> CreateCompositeMesh(const std::vector<Block>& blocks);

}       // namespace mc::world::chunk

