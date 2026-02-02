#pragma once

#include <glm/glm.hpp>

namespace ShaderUtils {
constexpr int kMaxBones = 100;

struct alignas(16) BoneBlock {
  BoneBlock() {
    // Initialize all bone matrices to identity
    for (int i = 0; i < kMaxBones; ++i) {
      _finalBones[i] = glm::mat4(1.0f);
    }
  }

  // Array of bone transformation matrices
  // Each mat4 is 16-byte aligned in std140 layout
  alignas(16) glm::mat4 _finalBones[kMaxBones];
};

static_assert(sizeof(BoneBlock) % 16 == 0);
static_assert(alignof(BoneBlock) == 16, "BoneBlock must be 16-byte aligned");
}  // namespace ShaderUtils
