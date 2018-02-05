/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "utility/FileRead.hpp"

#include "logger/GlobalLogger.hpp"
#include "ShaderError.hpp"
#include "ShaderProgramError.hpp"

namespace mc {

class ShaderProgram {

 public:

                ShaderProgram();
                ~ShaderProgram();
    void        AddVertexShader(const std::string& filePath);
    void        AddFragmentShader(const std::string& filePath);
    void        Link();
    void        Use();
    void        SetMVPMatrix(const glm::mat4& mvp);
    GLuint      GetId() const { return programId; }

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
