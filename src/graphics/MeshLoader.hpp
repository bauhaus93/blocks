/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>
#include <regex>
#include <iterator>
#include <vector>
#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "glad/Glad.h"
#include "utility/FileRead.hpp"
#include "logger/GlobalLogger.hpp"

namespace mc {

void LoadMeshDataFromFile(const std::string& filename,
                          std::vector<glm::vec3>& vertices,
                          std::vector<glm::vec2>& uvs,
                          std::vector<glm::vec3>& normals,
                          std::vector<uint32_t>& indices);


struct VertexData {
                VertexData(const glm::vec3& vertex_,
                           const glm::vec2& uv_,
                           const glm::vec3& normal_);

    bool        operator<(const VertexData& other) const;

    glm::vec3   vertex;
    glm::vec2   uv;
    glm::vec3   normal;

};

}    // namespace mc
