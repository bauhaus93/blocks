/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "FileNotFoundError.hpp"

namespace mc {

std::string ReadFile(const std::string& filePath);

}   // namespace mc
