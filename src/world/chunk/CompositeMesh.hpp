/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "logger/GlobalLogger.hpp"
#include "utility/Defs.hpp"
#include "utility/Point3.hpp"
#include "world/Direction.hpp"
#include "world/Block.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Quad.hpp"
#include "mesh/Triangle.hpp"

namespace mc::world::chunk {

mesh::Mesh CreateCompositeMesh(const Map3D<Block>& blocks);

}       // namespace mc::world::chunk

