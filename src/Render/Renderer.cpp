#include "Renderer.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <unordered_map>

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Render {
namespace {
const std::unordered_map<Renderer::DataType, VertexBuffer::_e_DataType>
    kRendererToVertexBuffer{
        {Renderer::Float, VertexBuffer::_e_DataType::Float},
        {Renderer::Float2, VertexBuffer::_e_DataType::Float2},
        {Renderer::Float3, VertexBuffer::_e_DataType::Float3},
        {Renderer::Float4, VertexBuffer::_e_DataType::Float4},
        {Renderer::Int, VertexBuffer::_e_DataType::Int},
        {Renderer::Int2, VertexBuffer::_e_DataType::Int2},
        {Renderer::Int3, VertexBuffer::_e_DataType::Int3},
        {Renderer::Int4, VertexBuffer::_e_DataType::Int4},
    };
}

// This must be called before upload(). The layout determines how the buffer
// contents are interpreted and passed to the vertex shader (e.g., positions,
// colors, UVs, normals).
void Renderer::setLayout(const std::vector<DataType>& layout) {
  _layout.clear();
  std::transform(layout.begin(), layout.end(), std::back_inserter(_layout),
                 [](auto elem) { return kRendererToVertexBuffer.at(elem); });
}

// This controls how the GPU interprets the vertex data during rendering.
void Renderer::setDrawMode(GLenum mode) { _drawMode = mode; }

// This must be called after setting the layout and filling the vertex buffer.
// It creates a VertexBuffer with the current layout and attaches it to the VAO.
void Renderer::upload() {
  if (_layout.empty()) {
    assert(false && "Layout is empty! Dont know how to interpret buffer!");
  }

  VertexBuffer VBO(_elementsBuff, _layout, VertexBuffer::_e_Usage::Static);
  _VAO.addBuffer(VBO);
}

// Stores the given indices and creates an IndexBuffer bound to the VAO.
// Call this before drawElements() if you plan to use indexed drawing.
void Renderer::setIndices(const std::vector<GLuint>& indices) {
  IndexBuffer IBO(indices);
  _indicesCount = indices.size();

  _VAO.setIndexBuffer(IBO);
}

// This uses non-indexed rendering (glDrawArrays) and requires vertex data
// to have been uploaded beforehand.
void Renderer::drawArrays() {
  _VAO.bind();
  glDrawArrays(_drawMode, 0, static_cast<GLsizei>(_vertexCount));
  _VAO.unbind();
}

// This uses indexed rendering (glDrawElements), so make sure to call
// setIndices() before this. Requires a valid layout and uploaded vertex data.
void Renderer::drawElements() {
  if (_indicesCount == 0) {
    assert(false &&
           "Cannot call drawElements(): index buffer has not been set.");
  }
  _VAO.bind();
  glDrawElements(_drawMode, static_cast<GLsizei>(_indicesCount), GL_UNSIGNED_INT, nullptr);
  _VAO.unbind();
}

}  // namespace Render