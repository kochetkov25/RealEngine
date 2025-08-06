#pragma once

#include "AlignUtils.h"

namespace ShaderUtils {
struct alignas(16) CameraBlock {
  CameraBlock(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
              const glm::vec3& cameraPosition)
      : _viewMat(viewMatrix),
        _projMat(projectionMatrix),
        _camPosition(cameraPosition) {}

  STD140(glm::mat4, _viewMat);
  STD140(glm::mat4, _projMat);
  STD140(glm::vec3, _camPosition);
};

static_assert(sizeof(CameraBlock) % 16 == 0);
}  // namespace ShaderUtils