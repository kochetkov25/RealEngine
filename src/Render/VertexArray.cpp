
#include "VertexArray.h"

#include "IndexBuffer.h"
#include "glad/glad.h"

namespace Render {

// Constructor: generates a new vertex array object
Render::VertexArray::VertexArray() { glGenVertexArrays(1, &_id); }

// Destructor: deletes the vertex array object
Render::VertexArray::~VertexArray() { glDeleteVertexArrays(1, &_id); }

VertexArray::VertexArray(VertexArray&& other) noexcept
    : _id(other._id), _layoutLocation(other._layoutLocation), _indexCount(other._indexCount) {
  other._id = 0;
  other._layoutLocation = 0;
  other._indexCount = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
  if (this != &other) {
    if (_id != 0) {
      glDeleteVertexArrays(1, &_id);
    }

    _id = other._id;
    _layoutLocation = other._layoutLocation;
    _indexCount = other._indexCount;

    other._id = 0;
    other._layoutLocation = 0;
    other._indexCount = 0;
  }
  return *this;
}

// Bind the vertex array object
void Render::VertexArray::bind() const { glBindVertexArray(_id); }

// Unbind the vertex array object
void Render::VertexArray::unbind() { glBindVertexArray(0); }

void Render::VertexArray::addBuffer(VertexBuffer& vertexBuffer) {
  bind();
  vertexBuffer.bind();

  GLuint currentLocation = _layoutLocation;

  for (const VertexBuffer::BufferElement& element : vertexBuffer._elements) {
    glEnableVertexAttribArray(currentLocation);

    auto componentType = static_cast<GLenum>(element.componentType);
    if (componentType == GL_UNSIGNED_INT) {
      glVertexAttribIPointer(currentLocation, element.size, componentType, vertexBuffer.getStride(),
                             reinterpret_cast<const void*>(element.offset));
    } else {
      glVertexAttribPointer(currentLocation, element.size, componentType, GL_FALSE, vertexBuffer.getStride(),
                            reinterpret_cast<const void*>(element.offset));
    }

    currentLocation++;
  }

  _layoutLocation = currentLocation;

  unbind();
}

void VertexArray::setIndexBuffer(IndexBuffer& indexBuffer) {
  bind();
  indexBuffer.bind();

  _indexCount = indexBuffer._count;
}

}  // namespace Render
