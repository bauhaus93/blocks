/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/FileRead.hpp"
#include "logger/GlobalLogger.hpp"
#include "application/ApplicationError.hpp"
#include "Image.hpp"

namespace mc {

Image ReadBitmap(const std::string& filePath);


}   // namespace mc
