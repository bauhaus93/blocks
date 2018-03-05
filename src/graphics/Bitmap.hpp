/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "glad/Glad.h"

#include "logger/GlobalLogger.hpp"
#include "ApplicationError.hpp"

namespace mc {


GLuint ReadBitmap(const std::string& filePath);


}   // namespace mc
