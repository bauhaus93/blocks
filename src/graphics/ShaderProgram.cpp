/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ShaderProgram.hpp"

namespace mc {

static GLuint LoadShader(const std::string& filePath, GLenum shaderType) {
    GLuint id = glCreateShader(shaderType);

    DEBUG("Compiling shader \"", filePath, "\"...");

    std::string code = ReadFile(filePath);

    const char* ptr = code.c_str();
    glShaderSource(id, 1, &ptr, nullptr);
    glCompileShader(id);

    GLint result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        throw ShaderError(__FUNCTION__, id);
    }
    else {
        switch (shaderType) {
        case GL_VERTEX_SHADER: INFO("Compiled vertex shader");  break;
        case GL_FRAGMENT_SHADER: INFO("Compiled fragment shader");  break;
        default: INFO("Compiled shader");   break;
        }
    }
    return id;
}

ShaderProgram::ShaderProgram():
    programId { glCreateProgram() },
    vertexShader { 0 },
    fragmentShader { 0 },
    mvpHandle { 0 } {
}

void ShaderProgram::AddVertexShader(const std::string& filePath) {
    vertexShader = LoadShader(filePath, GL_VERTEX_SHADER);
}

void ShaderProgram::AddFragmentShader(const std::string& filePath) {
    fragmentShader = LoadShader(filePath, GL_FRAGMENT_SHADER);
}

void ShaderProgram::Link() {
    DEBUG("Linking shader program...");

    LinkSetup();

    glLinkProgram(programId);

    GLint result = GL_FALSE;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        throw ShaderProgramError(__FUNCTION__, programId);
    }

    LinkCleanup();
    INFO("Linked shader program");

    LoadMVPHandle();
}

void ShaderProgram::LoadMVPHandle() {
    mvpHandle = glGetUniformLocation(programId, "MVP");
    if (mvpHandle == -1) {
        throw ShaderProgramError(__FUNCTION__, "Could not get mvp handle");
    }
}

void ShaderProgram::SetMVPMatrix(const glm::mat4& mvp) {
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &mvp[0][0]);
}


void ShaderProgram::Use() {
    glUseProgram(programId);
}

void ShaderProgram::LinkSetup() {
    if (vertexShader != 0) {
        glAttachShader(programId, vertexShader);
        DEBUG("Attached vertex shader");
    }

    if (fragmentShader != 0) {
        glAttachShader(programId, fragmentShader);
        DEBUG("Attached fragment shader");
    }
}

void ShaderProgram::LinkCleanup() {
    if (vertexShader != 0) {
        glDetachShader(programId, vertexShader);
        glDeleteShader(vertexShader);
        vertexShader = 0;
    }
    if (fragmentShader != 0) {
        glDetachShader(programId, fragmentShader);
        glDeleteShader(fragmentShader);
        fragmentShader = 0;
    }
}


}   // namespace mc
