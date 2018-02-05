/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <regex>
#include <iterator>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "utility/FileRead.hpp"
#include "logger/GlobalLogger.hpp"

namespace mc {

void LoadMeshDataFromFile(const std::string& filename,
                          std::vector<GLfloat>& vertices,
                          std::vector<GLfloat>& uvs,
                          std::vector<GLfloat>& normals);
}    // namespace mc
