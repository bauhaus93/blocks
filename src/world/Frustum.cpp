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
            plane[2 * i][j] = clip[3][j] + clip[i][j];      //LEFT / BOTTOM / NEAR
            plane[2 * i + 1][j] = clip[3][j] - clip[i][j];  //RIGHT / TOP / FAR
        }
    }
    for (int i = 0; i < 6; i++) {
        plane[i] = glm::normalize(plane[i]);
    }
}

Intersection Frustum::Intersects(const BoundingBox& box) const {
    const glm::vec3& center = box.GetCenter();
    const glm::vec3& extent = box.GetExtent();

    bool intersect = false;
    for (int i = 0; i < 6; i++) {
        glm::vec3 nVertex(center);
        for (int j = 0; j < 3; j++) {
            if (plane[i][j] < 0) {
                nVertex[j] += extent[j];
            } else {
                nVertex[j] -= extent[j];
            }
        }
        glm::vec3 normal(plane[i][0], plane[i][1], plane[i][2]);
        float nDistance = glm::dot(normal, nVertex) + plane[i][3];
        if (nDistance < 0.0f) {
            return Intersection::OUTSIDE;
        } else {
            glm::vec3 pVertex(center);
            for (int j = 0; j < 3; j++) {
                if (plane[i][j] < 0) {
                    pVertex[j] -= extent[j];
                } else {
                    pVertex[j] += extent[j];
                }
            }
            float pDistance = glm::dot(normal, pVertex) + plane[i][3];
            if (pDistance > 0.0f) {
                intersect = true;
            }
        }
    }

    if (intersect) {
        return Intersection::PARTIAL;
    }
    return Intersection::INSIDE;  
}

}   // namespace mc::world