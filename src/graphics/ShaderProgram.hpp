/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>

#include "glm/mat4x4.hpp"

#include "glad/glad.h"
#include "utility/FileRead.hpp"
#include "utility/Color.hpp"

#include "logger/GlobalLogger.hpp"
#include "ShaderError.hpp"
#include "ShaderProgramError.hpp"

namespace blocks {

struct FogData {
    GLint density;
    GLint color;
};

class ShaderProgram {

 public:

                ShaderProgram();
                ~ShaderProgram();
    void        AddVertexShader(const std::string& filePath);
    void        AddFragmentShader(const std::string& filePath);
    void        Link();
    void        Activate();
    void        Deactivate();
    void        SetMVPMatrix(const glm::mat4& mvp);
    void        SetFogDensity(float density);
    void        SetFogColor(Color color);
    GLuint      GetId() const { return programId; }

 private:
    GLuint      programId;
    GLuint      vertexShader;
    GLuint      fragmentShader;
    GLint       mvpHandle;
    GLint       uniformTexture;
    FogData     fog;

    void        LinkSetup();
    void        LinkCleanup();
    void        LoadMVPHandle();
    void        LoadUniformTexture();
    void        LoadUniformFog();

};

}   // namespace blocks
