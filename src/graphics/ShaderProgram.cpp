/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ShaderProgram.hpp"

namespace mc::graphics {

static GLuint LoadShader(const std::string& filePath, GLenum shaderType);

ShaderProgram::ShaderProgram():
    programId { glCreateProgram() },
    vertexShader { 0 },
    fragmentShader { 0 },
    mvpHandle { 0 },
    uniformTexture { 0 } {
    INFO("Creating shader program");
}

ShaderProgram::~ShaderProgram() {
    if (programId != 0) {
        INFO("Destroying shader program");
        glDeleteProgram(programId);
    }
}

void ShaderProgram::AddVertexShader(const std::string& filePath) {
    vertexShader = LoadShader(filePath, GL_VERTEX_SHADER);
}

void ShaderProgram::AddFragmentShader(const std::string& filePath) {
    fragmentShader = LoadShader(filePath, GL_FRAGMENT_SHADER);
}

void ShaderProgram::Link() {
    DEBUG("Linking shader program");

    LinkSetup();

    glLinkProgram(programId);

    GLint result = GL_FALSE;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        throw ShaderProgramError(__FUNCTION__, programId);
    }

    LinkCleanup();
    DEBUG("Linked shader program");

    LoadMVPHandle();
    LoadUniformTexture();
}

void ShaderProgram::LoadMVPHandle() {
    mvpHandle = glGetUniformLocation(programId, "MVP");
    if (mvpHandle == -1) {
        throw ShaderProgramError(__FUNCTION__,
                                 "Could not get mvp handle");
    }
}

void ShaderProgram::LoadUniformTexture() {
    uniformTexture = glGetUniformLocation(programId, "textureArray");
    if (uniformTexture == -1) {
        throw ShaderProgramError(__FUNCTION__,
                                 "Could not get uniform texture location");
    }
    glUniform1i(uniformTexture, 0); //TODO maybe move to other location to set each frame
}

void ShaderProgram::SetMVPMatrix(const glm::mat4& mvp) const {
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &mvp[0][0]);
}

void ShaderProgram::Use() {
    glUseProgram(programId);
}

void ShaderProgram::LinkSetup() {
    if (vertexShader != 0) {
        glAttachShader(programId, vertexShader);
        TRACE("Attached vertex shader");
    }

    if (fragmentShader != 0) {
        glAttachShader(programId, fragmentShader);
        TRACE("Attached fragment shader");
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

static GLuint LoadShader(const std::string& filePath, GLenum shaderType) {
    GLuint id = glCreateShader(shaderType);

    switch (shaderType) {
    case GL_VERTEX_SHADER: DEBUG("Compiling vertex shader: \"", filePath, "\"");  break;
    case GL_FRAGMENT_SHADER: DEBUG("Compiling fragment shader: \"", filePath, "\"");  break;
    default: DEBUG("Compiling shader: \"", filePath, "\"");   break;
    }

    std::string code = ReadFile(filePath);

    const char* ptr = code.c_str();
    glShaderSource(id, 1, &ptr, nullptr);
    glCompileShader(id);

    GLint result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        throw ShaderError(__FUNCTION__, id);
    } else {
        DEBUG("Compiled shader");
    }
    return id;
}


}   // namespace mc::graphics
