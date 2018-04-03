/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <map>
#include <functional>

#include "Point3.hpp"

namespace mc {

template<typename T>
using VecRef = std::vector<std::reference_wrapper<T>>;

template<typename T>
using Map3D = std::map<Point3i, T>;

template<typename T>
using MapRef3D = std::map<Point3i, std::reference_wrapper<T>>;

}   // namespace mc
