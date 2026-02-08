#pragma once

#include "VertexBuffer.h"

namespace Render {
/*forward declaration*/
class IndexBuffer;

class VertexArray {
 public:
  VertexArray();
  ~VertexArray();

  VertexArray(const VertexArray&) = delete;
  VertexArray& operator=(const VertexArray&) = delete;

  VertexArray(VertexArray&& other) noexcept;
  VertexArray& operator=(VertexArray&& other) noexcept;

  void addBuffer(VertexBuffer& vertexBuffer);

  void setIndexBuffer(IndexBuffer& indexBuffer);

  void bind() const;
  static void unbind();

  [[nodiscard]] size_t indexCount() const noexcept { return _indexCount; }
  [[nodiscard]] bool hasIndices() const noexcept { return _indexCount > 0; }

 private:
  unsigned int _id = 0;
  unsigned int _layoutLocation = 0;

  size_t _indexCount = 0;
};

}  // namespace Render