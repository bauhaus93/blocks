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
    bool intersect = false;
    for (int i = 0; i < 6; i++) {
        glm::vec3 normal(plane[i][0], plane[i][1], plane[i][2]);
        glm::vec3 nVertex(center);
        glm::vec3 pVertex(center);
        for (int i = 0; i < 3; i++) {
            if (normal[i] > 0) {
                if (nVertex[i] > 0) {
                    nVertex[i] -= extent[i];
                } else {
                    nVertex[i] += extent[i];
                }
            } else {
                if (nVertex[i] > 0) {
                    nVertex[i] += extent[i];
                } else {
                    nVertex[i] -= extent[i];
                }
            }

        if (glm::dot(nVertex, normal) + plane[i][3] < 0) {
            return Intersection::OUTSIDE;
        }
        if (glm::dot(pVertex, normal) + plane[i][3] < 0) {
            intersect = true;
        }
    }
    if (intersect) {
        return Intersection::PARTIAL;
    }
    return Intersection::INSIDE;
}

}   // namespace mc::world