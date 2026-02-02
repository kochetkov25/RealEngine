#pragma once
#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <string>

namespace Render {

class ShaderProgram {
 public:
  ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);

  ~ShaderProgram();

  bool isCompiled() const { return _isCompiled; }

  void use() const;

  void setTexUniform(const std::string &textureName, const GLint value);

  void setMatrix4Uniform(const std::string &vecName, const glm::mat4 &matrix);

  void setVec3Uniform(const std::string &matrixName, const glm::vec3 &vec3);

  void setVec2Uniform(const std::string &matrixName, const glm::vec2 &vec2);

  void setFloatUniform(const std::string &name, const float value);

  void setIntUniform(const std::string &name, const GLint value);

  void setBoolUniform(const std::string &name, bool value);

  void setArrayUniform(const std::string &name, const size_t size, const float *arr);

  [[nodiscard]] GLuint getShaderProgramID() const { return _ID; }

 private:
  bool createShader(const std::string &shaderSource, const GLenum shaderType, GLuint &shaderID);
  
  void bindMaterialSamplerSlots();

  bool _isCompiled = false;

  GLuint _ID = 0;
};

}  // namespace Render