#pragma once

#include <glad/glad.h>

namespace Render {

/*
 * 1. put data to buffer: set()
 * 2. bind buffer: bind() (connect buffer to binding point)
 * 3. call draw func
 */
template <typename T>
class UniformBuffer {
 public:
  UniformBuffer(GLuint bindingIndex) : UniformBuffer(sizeof(T), bindingIndex) {}

  ~UniformBuffer() { glDeleteBuffers(1, &_ubo); }

  UniformBuffer(const UniformBuffer&) = delete;
  UniformBuffer(UniformBuffer&&) = delete;

  UniformBuffer& operator=(const UniformBuffer&) = delete;
  UniformBuffer& operator=(const UniformBuffer&&) = delete;

  GLuint id() const { return _ubo; }
  GLuint binding() const { return _binding; }

  void rebind(GLuint bindingIndex) {
    _binding = bindingIndex;
    bind();
  }

  void set(const T& data, size_t offset = 0) {
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void bind() { glBindBufferBase(GL_UNIFORM_BUFFER, _binding, _ubo); }

 private:
  UniformBuffer(size_t size, GLuint bindingIndex) : _size(size), _binding(bindingIndex) {
    glGenBuffers(1, &_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  GLuint _ubo = 0;
  GLuint _binding = 0;
  size_t _size = 0;
};

}  // namespace Render