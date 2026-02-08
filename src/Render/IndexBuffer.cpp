#include "IndexBuffer.h"

namespace Render {

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &_id); }

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : _id(other._id), _count(other._count) {
  other._id = 0;
  other._count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
  if (this != &other) {
    if (_id != 0) {
      glDeleteBuffers(1, &_id);
    }
    _id = other._id;
    _count = other._count;
    other._id = 0;
    other._count = 0;
  }
  return *this;
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }

void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}  // namespace Render