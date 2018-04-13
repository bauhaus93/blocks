/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#include "logger/GlobalLogger.hpp"
#include "FileNotFoundError.hpp"

namespace mc {

std::string ReadFile(const std::string& filePath);
std::vector<uint8_t> ReadFileRaw(const std::string& filePath);
}   // namespace mc
