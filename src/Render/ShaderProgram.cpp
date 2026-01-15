#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Render {

ShaderProgram::ShaderProgram(const std::string &vertexShader,
                             const std::string &fragmentShader) {
  GLuint vertexShaderID;
  if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
    std::cerr << "Error compile VERTEX SHADER. (source: " << __FUNCTION__ << ")"
              << std::endl;
    return;
  }

  GLuint fragmentShaderID;
  if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
    std::cerr << "Error compile FRAGMENT SHADER. (source: " << __FUNCTION__
              << ")" << std::endl;
    glDeleteShader(vertexShaderID);
    return;
  }

  _ID = glCreateProgram();
  glAttachShader(_ID, vertexShaderID);
  glAttachShader(_ID, fragmentShaderID);
  glLinkProgram(_ID);

  GLint isSuccessLink;
  glGetProgramiv(_ID, GL_LINK_STATUS, &isSuccessLink);
  if (!isSuccessLink) {
    GLchar infoLog[1024];
    glGetProgramInfoLog(_ID, 1024, nullptr, infoLog);
    std::cerr << "Error link SHADER PROGRAM (source: " << __FUNCTION__ << ") \n"
              << infoLog << std::endl;
  } else {
    _isCompiled = true;
  }

  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(_ID); }

bool ShaderProgram::createShader(const std::string &shaderSource,
                                 const GLenum shaderType, GLuint &shaderID) {
  shaderID = glCreateShader(shaderType);
  const char *code = shaderSource.c_str();
  glShaderSource(shaderID, 1, &code, nullptr);
  glCompileShader(shaderID);

  GLint isSuccessCompile;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isSuccessCompile);
  if (!isSuccessCompile) {
    GLchar infoLog[1024];
    glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
    std::cerr << "Error compile shader (source: " << __FUNCTION__ << ") \n"
              << infoLog << std::endl;
    return false;
  }
  return true;
}

void ShaderProgram::use() const { glUseProgram(_ID); }

void ShaderProgram::setTexUniform(const std::string &textureName,
                                  const GLint value) {
  glUniform1i(glGetUniformLocation(_ID, textureName.c_str()), value);
}

void ShaderProgram::setMatrix4Uniform(const std::string &matrixName,
                                      const glm::mat4 &matrix) {
  const auto loc = glGetUniformLocation(_ID, matrixName.c_str());
  if (loc == -1) {
    assert(false && "Cannot find uniform location!");
  }
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Render::ShaderProgram::setVec3Uniform(const std::string &vecName,
                                           const glm::vec3 &vec3) {
  glUniform3f(glGetUniformLocation(_ID, vecName.c_str()), vec3.x, vec3.y,
              vec3.z);
}

void Render::ShaderProgram::setVec2Uniform(const std::string &vecName,
                                           const glm::vec2 &vec2) {
  glUniform2f(glGetUniformLocation(_ID, vecName.c_str()), vec2.x, vec2.y);
}

void Render::ShaderProgram::setFloatUniform(const std::string &name,
                                            const float value) {
  glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
}

void Render::ShaderProgram::setIntUniform(const std::string &name,
                                          const GLint value) {
  glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
}

void Render::ShaderProgram::setBoolUniform(const std::string &name,
                                           bool value) {
  // In GLSL, bool uniforms are set as integers (0 or 1)
  glUniform1i(glGetUniformLocation(_ID, name.c_str()), value ? 1 : 0);
}

void Render::ShaderProgram::setArrayUniform(const std::string &name,
                                            const size_t size,
                                            const float *arr) {
  glUniform1fv(glGetUniformLocation(_ID, name.c_str()),
               static_cast<GLsizei>(size), arr);
}
} // namespace Render