#pragma once

#include "AlignUtils.h"

namespace ShaderUtils {
constexpr int kMaxLigtsCount = 100;

struct alignas(16) LightData {
  LightData()
      : _lightPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
        _lightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
        _ambientFactor(glm::vec3(0.2f, 0.2f, 0.2f)),
        _diffuseFactor(glm::vec3(0.5f, 0.5f, 0.5f)),
        _specularFactor(glm::vec3(1.0f, 1.0f, 1.0f)),
        _constantFactor(1.0f),
        _linearFactor(0.09f),
        _quadFactor(0.032f) {}

  STD140(glm::vec3, _lightPosition);
  STD140(glm::vec3, _lightColor);

  STD140(glm::vec3, _ambientFactor);
  STD140(glm::vec3, _diffuseFactor);
  STD140(glm::vec3, _specularFactor);

  STD140(float, _constantFactor);
  STD140(float, _linearFactor);
  STD140(float, _quadFactor);
};

static_assert(sizeof(LightData) % 16 == 0);

struct alignas(16) LightBlock {
  LightBlock() : _count(0) {}

  LightData _data[kMaxLigtsCount];

  STD140(int, _count);
};

static_assert(sizeof(LightBlock) % 16 == 0);
}  // namespace ShaderUtils