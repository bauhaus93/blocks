/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ShaderProgramError.hpp"

namespace mc::graphics {

static std::string GetProgramError(GLuint programId) {
    int logLen;

    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);

    std::vector<char> msgVec(logLen + 1);
    glGetProgramInfoLog(programId, logLen, nullptr, &msgVec[0]);
    std::string msg(msgVec.begin(), msgVec.end());

    return msg;
}


ShaderProgramError::ShaderProgramError(const std::string& function,
                                       GLuint programId):
    OpenGLError { "ShaderProgramError", function, GetProgramError(programId) } {
}

ShaderProgramError::ShaderProgramError(const std::string& function,
                                      const std::string& description):
    OpenGLError { "ShaderProgramError", function, description } {

}


}   // namespace mc::graphics
