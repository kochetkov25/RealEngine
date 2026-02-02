#pragma once

#include <glad/glad.h>

#include <bit>
#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"

namespace Render {
static const size_t MAX_ELEMENTS = 1024 * 2;

static const size_t MAX_LAYOUT = 10;

class Renderer {
  friend class RendererFactory;

 public:
  enum DataType { Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, UInt4 };

  // Appends a vertex position (glm::vec3) to the buffer
  void vertex(const glm::vec3& pos) {
    _elementsBuff.emplace_back(pos.x);
    _elementsBuff.emplace_back(pos.y);
    _elementsBuff.emplace_back(pos.z);
    _vertexCount++;
  }

  // Appends UV coordinates (glm::vec2) to the buffer
  void vertexUV(const glm::vec2& uv) {
    _elementsBuff.emplace_back(uv.x);
    _elementsBuff.emplace_back(uv.y);
  }

  // Appends a normal vector (glm::vec3) to the buffer
  void normal(const glm::vec3& n) {
    _elementsBuff.emplace_back(n.x);
    _elementsBuff.emplace_back(n.y);
    _elementsBuff.emplace_back(n.z);
  }

  // Adds RGBA vertex color (glm::vec4, normalized 0-1 range) to the buffer
  void color(const glm::vec4& c) {
    _elementsBuff.emplace_back(c.r);
    _elementsBuff.emplace_back(c.g);
    _elementsBuff.emplace_back(c.b);
    _elementsBuff.emplace_back(c.a);
  }

  // Adds RGBA vertex color (glm::vec3, alpha defaults to 1.0) to the buffer
  void color(const glm::vec3& c, float a = 1.0f) {
    _elementsBuff.emplace_back(c.r);
    _elementsBuff.emplace_back(c.g);
    _elementsBuff.emplace_back(c.b);
    _elementsBuff.emplace_back(a);
  }

  // Adds bone IDs (glm::uvec4) for skeletal animation (max 4 bones per vertex)
  // Bone IDs are indices into the bone transformation matrix array
  void boneIDs(const glm::uvec4& ids) {
    _elementsBuff.emplace_back(std::bit_cast<float>(ids.x));
    _elementsBuff.emplace_back(std::bit_cast<float>(ids.y));
    _elementsBuff.emplace_back(std::bit_cast<float>(ids.z));
    _elementsBuff.emplace_back(std::bit_cast<float>(ids.w));
  }

  // Adds bone weights (glm::vec4) for skeletal animation (max 4 bones per vertex)
  // Weights should sum to 1.0 for proper skinning
  void boneWeights(const glm::vec4& weights) {
    _elementsBuff.emplace_back(weights.x);
    _elementsBuff.emplace_back(weights.y);
    _elementsBuff.emplace_back(weights.z);
    _elementsBuff.emplace_back(weights.w);
  }

  // Appends a vertex (x, y, z) to the buffer
  template <typename T>
  void vertex3(T x, T y, T z) {
    _elementsBuff.emplace_back(static_cast<float>(x));
    _elementsBuff.emplace_back(static_cast<float>(y));
    _elementsBuff.emplace_back(static_cast<float>(z));

    _vertexCount++;
  }

  // Adds RGBA vertex color (0-255 range) to the buffer
  template <typename _T>
  void color4(_T r, _T g, _T b, _T a = 1.f) {
    _elementsBuff.emplace_back(static_cast<float>(r) / 255.f);
    _elementsBuff.emplace_back(static_cast<float>(g) / 255.f);
    _elementsBuff.emplace_back(static_cast<float>(b) / 255.f);
    _elementsBuff.emplace_back(static_cast<float>(a));
  }

  // Appends sprite UV coordinates (normalized 0-1) to the buffer
  template <typename _T>
  void vertexUV(_T U, _T V) {
    _elementsBuff.emplace_back(static_cast<float>(U));
    _elementsBuff.emplace_back(static_cast<float>(V));
  }

  // Adds bone IDs (4 unsigned integers) for skeletal animation
  // Parameters: i0, i1, i2, i3 - bone indices (max 4 bones per vertex)
  void boneIDs(unsigned int i0, unsigned int i1, unsigned int i2, unsigned int i3) {
    _elementsBuff.emplace_back(std::bit_cast<float>(i0));
    _elementsBuff.emplace_back(std::bit_cast<float>(i1));
    _elementsBuff.emplace_back(std::bit_cast<float>(i2));
    _elementsBuff.emplace_back(std::bit_cast<float>(i3));
  }

  // Adds bone weights (4 floats) for skeletal animation
  // Parameters: w0, w1, w2, w3 - weights for each bone (should sum to 1.0)
  void boneWeights(float w0, float w1, float w2, float w3) {
    _elementsBuff.emplace_back(w0);
    _elementsBuff.emplace_back(w1);
    _elementsBuff.emplace_back(w2);
    _elementsBuff.emplace_back(w3);
  }

  // Sets the vertex layout describing the structure of vertex data.
  void setLayout(const std::vector<DataType>& layout);

  // Sets the OpenGL primitive draw mode (e.g., GL_TRIANGLES, GL_LINES).
  void setDrawMode(GLenum mode);

  // Uploads the vertex data and layout to the GPU.
  void upload();

  // Sets the index buffer for indexed rendering.
  void setIndices(const std::vector<GLuint>& indices);

  // Draws the uploaded vertex buffer using the configured draw mode.
  void drawArrays();

  // Draws geometry using the uploaded vertex and index buffers.
  void drawElements();

 private:
  // Constructor (only accessible by RendererFactory)
  explicit Renderer() : _drawMode(GL_TRIANGLES), _vertexCount(0), _indicesCount(0) {
    _elementsBuff.reserve(MAX_ELEMENTS);
    _layout.reserve(MAX_LAYOUT);
  }

  std::vector<float> _elementsBuff;
  size_t _vertexCount;
  size_t _indicesCount;

  GLenum _drawMode;

  VertexArray _VAO;

  std::vector<VertexBuffer::BufferElement> _layout;
};
}  // namespace Render
