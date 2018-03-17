/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Frustum.hpp"

namespace mc::world {

Frustum::Frustum(const glm::mat4& view, const glm::mat4& projection):
    clip { projection * view } {
    UpdatePlanes();
}
void Frustum::Update(const glm::mat4& view, const glm::mat4& projection) {
    clip = projection * view;
    UpdatePlanes();
}

void Frustum::UpdatePlanes() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            //plane[2 * i][j] = clip[3][j] + clip[i][j];      //LEFT / BOTTOM / NEAR
            //plane[2 * i + 1][j] = clip[3][j] - clip[i][j];  //RIGHT / TOP / FAR
            plane[2 * i][j] = clip[j][3] + clip[j][i];      //LEFT / BOTTOM / NEAR
            plane[2 * i + 1][j] = clip[j][3] - clip[j][i];  //RIGHT / TOP / FAR
        }
    }
    
    for (int i = 0; i < 6; i++) {
        plane[i] = glm::normalize(plane[i]);
        //static const std::array<std::string, 6> NAME { "LEFT", "RIGHT", "BOTTOM", "TOP", "NEAR", "FAR" };
        //INFO(NAME[i], ": ", plane[i][0], "/", plane[i][1], "/", plane[i][2], "/", plane[i][3]);
    }
}

//TODO refine: less vertices to check with n/p vertices, check for Intersection::PARTIAL
Intersection Frustum::Intersects(const BoundingBox& box) const {
    const glm::vec3& center = box.GetCenter();
    const glm::vec3& extent = box.GetExtent();

    for (int i = 0; i < 6; i++) {
        int outCount = 0;
        for (int j = 0; j < 8; j++) {
            glm::vec4 vertex(center, 1.0f);
            for (int k = 0; k < 3; k++) {
                if ((j & (k + 1)) == 0) {
                    vertex[k] -= extent[k];
                } else {
                    vertex[k] += extent[k];
                }
            }
            if (glm::dot(plane[i], vertex) < 0.0f) {
                outCount++;
            }
        }
        if (outCount == 8) {
            return Intersection::OUTSIDE;
        }
    }
    return Intersection::INSIDE;
}

}   // namespace mc::world