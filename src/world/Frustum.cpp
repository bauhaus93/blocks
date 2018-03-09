/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Frustum.hpp"

namespace mc::world {

Frustum::Frustum(const glm::mat4& view, const glm::mat4& projection) {
    glm::mat4 clip = projection * view;

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


}   // namespace mc::world