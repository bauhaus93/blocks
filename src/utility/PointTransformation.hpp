/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/transform.hpp"

#include "Point3.hpp"

namespace mc {

glm::mat4 CreateTranslationMatrix(const glm::vec3& point);

template<typename T>
glm::vec3 CreateVec(const Point3<T>& point) {
    return glm::vec3(point[0], point[1], point[2]);
}

template<typename T>
glm::mat4 CreateTranslationMatrix(const Point3<T>& point) {
    return glm::translate(glm::mat4(1.0f), CreateVec(point));
}

template<typename T>
glm::mat4 CreateRotationMatrix(const Point3<T>& point) {
    return glm::rotate(point[0], glm::vec3(1, 0, 0)) *
           glm::rotate(point[1], glm::vec3(0, 1, 0)) *
           glm::rotate(point[2], glm::vec3(0, 0, 1));
}

template<typename T>
glm::vec3 CreateDirection(const Point3<T>& rotation) {
    return glm::normalize(glm::vec3 {
        sin(rotation[1]) * cos(rotation[0]),
        sin(rotation[1]) * sin(rotation[0]),
        cos(rotation[1]) });
}

}   // namespace mc
