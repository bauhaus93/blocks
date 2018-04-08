/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ShaderError.hpp"

namespace mc::graphics {

static std::string GetShaderError(GLuint shaderId) {
    int logLen;

    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLen);

    std::vector<char> msgVec(logLen + 1);
    glGetShaderInfoLog(shaderId, logLen, nullptr, &msgVec[0]);
    std::string msg(msgVec.begin(), msgVec.end());

    return msg;
}

ShaderError::ShaderError(const std::string& function, GLuint shaderId):
    OpenGLError { "ShaderError", __FUNCTION__, GetShaderError(shaderId) } {
}


}   // namespace mc::graphics
