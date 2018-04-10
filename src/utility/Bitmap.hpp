/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "utility/Point2.hpp"
#include "logger/GlobalLogger.hpp"
#include "ApplicationError.hpp"

namespace mc {

struct BitmapResult {
    std::vector<uint8_t> data;
    Point2i size;
};

BitmapResult ReadBitmap(const std::string& filePath);


}   // namespace mc
