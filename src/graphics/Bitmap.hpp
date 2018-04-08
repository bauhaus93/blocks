/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "utility/Point2.hpp"
#include "logger/GlobalLogger.hpp"
#include "glad/Glad.h"
#include "ApplicationError.hpp"

namespace mc::graphics {

struct BitmapResult {
    GLuint id;
    Point2i size;
};

BitmapResult ReadBitmap(const std::string& filePath);


}   // namespace mc::graphics
