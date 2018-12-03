/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Vertex.hpp"

namespace blocks {

Vertex::Vertex(Point3f pos_, Point3f uv_, Point3f normal_):
    pos { pos_ },
    uv { uv_ },
    normal { normal_ } {
}

bool Vertex::operator<(const Vertex& other) const {
    for (uint8_t i = 0; i < 3; i++) {
        if (pos[i] < other.pos[i]) {
            return true;
        } else if (pos[i] > other.pos[i]) {
            return false;
        }
    }
    for (uint8_t i = 0; i < 2; i++) {
        if (uv[i] < other.uv[i]) {
            return true;
        } else if (uv[i] > other.uv[i]) {
            return false;
        }
    }
    for (uint8_t i = 0; i < 3; i++) {
        if (normal[i] < other.normal[i]) {
            return true;
        } else if (normal[i] > other.normal[i]) {
            return false;
        }
    }
    return false;
}

glm::vec3 Vertex::GetGlmPos() const {
    return glm::vec3(pos[0], pos[1], pos[2]);
}

glm::vec3 Vertex::GetGlmUV() const {
    return glm::vec3(uv[0], uv[1], uv[2]);
}

glm::vec3 Vertex::GetGlmNormal() const {
    return glm::vec3(normal[0], normal[1], normal[2]);
}

bool Vertex::InVolume(const Volume& volume) const {
    return volume.Contains(pos);
}

float Vertex::GetDistance(const Vertex& other) const {
    return pos.GetDistance(other.pos);
}

std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
    os << "Vertex(";
    os << "pos = (" << vertex.GetPos() << ")";
    os << ", uv = (" << vertex.GetUV() << ")";
    os << ", normal = (" << vertex.GetNormal() << ")";
    os << ")";
    return os;
}

}       // namespace blocks
