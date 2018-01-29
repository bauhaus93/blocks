/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "logger/GlobalLogger.hpp"
#include "ShaderError.hpp"
#include "ShaderProgramError.hpp"

namespace mc {

class ShaderProgram {

 public:

                ShaderProgram();
    void        AddVertexShader(const std::string& filePath);
    void        AddFragmentShader(const std::string& filePath);
    void        Link();
    void        Use();
    void        SetMVPMatrix(const glm::mat4& mvp);

 private:
    GLuint      programId;
    GLuint      vertexShader;
    GLuint      fragmentShader;
    GLint       mvpHandle;

    void        LinkSetup();
    void        LinkCleanup();
    void        LoadMVPHandle();

};

}   // namespace mc
