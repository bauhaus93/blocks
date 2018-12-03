/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>
#include <map>
#include <vector>

#include "Face.hpp"

namespace blocks{

typedef std::array<std::map<uint8_t, std::vector<Face>>, 3> LayerFaces;

}   // namespace chunk
