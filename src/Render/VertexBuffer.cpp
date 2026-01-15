#include "VertexBuffer.h"

#include "../Modules/Logger.h"

namespace Render {

// Map custom Usage enum to OpenGL GLenum
GLenum Render::VertexBuffer::getGLenum(const _e_Usage usage) {
  switch (usage) {
  case _e_Usage::Static:
    return GL_STATIC_DRAW;
  case _e_Usage::Dynamic:
    return GL_DYNAMIC_DRAW;
  case _e_Usage::Stream:
    return GL_STREAM_DRAW;
  default:
    Core::Logger::warning("VertexBuffer", "Unknown buffer usage type");
    return GL_STREAM_DRAW;
  }
}

// Destructor: deletes the OpenGL buffer
Render::VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &_id); }

// Bind the vertex buffer
void Render::VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, _id); }

// Unbind the vertex buffer
void Render::VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// Get OpenGL component type based on custom DataType
unsigned int Render::VertexBuffer::getComponentType(_e_DataType type) {
  switch (type) {
  case Render::VertexBuffer::_e_DataType::Float:
  case Render::VertexBuffer::_e_DataType::Float2:
  case Render::VertexBuffer::_e_DataType::Float3:
  case Render::VertexBuffer::_e_DataType::Float4:
    return GL_FLOAT;
  case Render::VertexBuffer::_e_DataType::Int:
  case Render::VertexBuffer::_e_DataType::Int2:
  case Render::VertexBuffer::_e_DataType::Int3:
  case Render::VertexBuffer::_e_DataType::Int4:
    return GL_INT;
  default:
    Core::Logger::warning("VertexBuffer", "Unknown data type");
    return GL_FLOAT;
  }
}

// Get the number of components for a given DataType
unsigned int Render::VertexBuffer::getElementSize(_e_DataType type) {
  switch (type) {
  case Render::VertexBuffer::_e_DataType::Float:
  case Render::VertexBuffer::_e_DataType::Int:
    return 1;
  case Render::VertexBuffer::_e_DataType::Float2:
  case Render::VertexBuffer::_e_DataType::Int2:
    return 2;
  case Render::VertexBuffer::_e_DataType::Float3:
  case Render::VertexBuffer::_e_DataType::Int3:
    return 3;
  case Render::VertexBuffer::_e_DataType::Float4:
  case Render::VertexBuffer::_e_DataType::Int4:
    return 4;
  default:
    Core::Logger::warning("VertexBuffer", "Unknown data type for element size");
    return 0;
  }
}

// Get the stride (size of a single vertex)
size_t Render::VertexBuffer::getStride() { return _stride; }
} // namespace Render
