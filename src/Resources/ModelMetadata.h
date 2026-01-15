#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Resources {

/**
 * @brief Metadata structure for loaded 3D models
 *
 * Contains bounding box information, optimization hints,
 * and other metadata useful for rendering and culling.
 */
struct ModelMetadata {
  // Bounding box information
  glm::vec3 boundingBoxMin{0.0f, 0.0f, 0.0f};
  glm::vec3 boundingBoxMax{0.0f, 0.0f, 0.0f};
  glm::vec3 boundingBoxCenter{0.0f, 0.0f, 0.0f};
  float boundingRadius{0.0f};

  // Mesh statistics
  uint32_t totalVertices{0};
  uint32_t totalIndices{0};
  uint32_t meshCount{0};
  uint32_t textureCount{0};

  // Optimization hints
  bool hasAnimations{false};
  bool hasBones{false};
  bool hasTangents{false};
  bool hasBitangents{false};

  // File information
  std::string sourceFilePath;
  std::string modelName;

  /**
   * @brief Calculate bounding box from vertex positions
   */
  void calculateBounds(const std::vector<glm::vec3> &vertices) noexcept {
    if (vertices.empty()) {
      return;
    }

    boundingBoxMin = vertices[0];
    boundingBoxMax = vertices[0];

    for (const auto &vertex : vertices) {
      boundingBoxMin = glm::min(boundingBoxMin, vertex);
      boundingBoxMax = glm::max(boundingBoxMax, vertex);
    }

    boundingBoxCenter = (boundingBoxMin + boundingBoxMax) * 0.5f;

    // Calculate bounding radius
    float maxDistance = 0.0f;
    for (const auto &vertex : vertices) {
      const float distance = glm::length(vertex - boundingBoxCenter);
      maxDistance = glm::max(maxDistance, distance);
    }
    boundingRadius = maxDistance;
  }

  /**
   * @brief Get the size of the bounding box
   */
  glm::vec3 getBoundingBoxSize() const noexcept {
    return boundingBoxMax - boundingBoxMin;
  }
};

} // namespace Resources
