#pragma once

#include <glad/glad.h>

#include <vector>

namespace Render {

class VertexBuffer {
 public:
  friend class VertexArray;
  friend class IndexBuffer;

  // Enum for buffer usage patterns
  enum class _e_Usage { Static, Dynamic, Stream };

  // Enum for data types of buffer elements
  enum class _e_DataType {
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4
  };

  // Structure to describe a single element within the vertex buffer (e.g., position, color, UV)
  struct BufferElement {
    _e_DataType dataType;       // Data type of the component
    uint32_t componentType;  // OpenGL component type (e.g., GL_FLOAT, GL_INT)
    size_t size;             // Number of components (e.g., 3 for Float3)
    size_t offset;           // Offset of this element within the vertex

    BufferElement(_e_DataType type)
        : dataType(type), componentType(GL_FLOAT), size(0), offset(0){};
  };

  // Constructor and destructor
  template <typename _T>
  VertexBuffer(const std::vector<_T>& data,
               std::vector<BufferElement>& elements,
               _e_Usage usage = _e_Usage::Static)
      : _elements(std::move(elements)), _usage(usage) {
    size_t offset = 0;
    // Calculate component type, size, offset, and stride for each element
    for (auto& element : _elements) {
      element.componentType = getComponentType(element.dataType);
      element.size = getElementSize(element.dataType);
      element.offset = offset;
      offset += element.size * sizeof(_T);  // Accumulate offset in bytes
      _stride += element.size * sizeof(_T);  // Accumulate stride in bytes
    }

    glGenBuffers(1, &_id);               // Generate buffer ID
    glBindBuffer(GL_ARRAY_BUFFER, _id);  // Bind the buffer
    // Upload data to the GPU
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(),
                 getGLenum(usage));  // Allocate and initialize buffer data
  }

  // Update buffer data
  template <typename _T>
  void updateData(const std::vector<_T>& data) {
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(),
                 getGLenum(_usage));
  }

  // Destructor
  ~VertexBuffer();

  // Delete copy and move constructors/assignment operators
  VertexBuffer() = delete;
  VertexBuffer(const VertexBuffer&) = delete;
  VertexBuffer(VertexBuffer&&) = delete;
  VertexBuffer& operator=(const VertexBuffer&) = delete;
  VertexBuffer& operator=(const VertexBuffer&&) = delete;

  void bind() const;
  static void unbind();
  size_t getStride();

 private:
  unsigned int _id = 0;  // OpenGL buffer ID
  std::vector<BufferElement> _elements;  // Layout of buffer elements
  size_t _stride = 0;  // Total size of a single vertex in bytes
  _e_Usage _usage;

  static GLenum getGLenum(const _e_Usage usage);

  // Get OpenGL component type for a given DataType
  unsigned int getComponentType(_e_DataType type);
  // Get size (number of components) for a given DataType
  unsigned int getElementSize(_e_DataType type);
};

}  // namespace Render
