#pragma once

#include "AlignUtils.h"

namespace ShaderUtils {
constexpr int kMaxLightsCount = 100;

struct alignas(16) LightData {
  LightData()
      : _position(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)),         // .xyz = Pos, .w = Type (0 = Point, 1 = Directional)
        _color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),            // .xyz = RGB, .w = Intensity (1.0)
        _attenuation(glm::vec4(1.0f, 0.09f, 0.032f, 20.0f)),  // .x = Const, .y = Lin, .z = Quad, .w = Radius
        _factors(glm::vec4(0.2f, 0.5f, 1.0f, 0.0f))           // .x = Amb, .y = Diff, .z = Spec, .w = Pad
  {}

  // .xyz = Light Position
  // .w   = Light Type (0.0 = Point, 1.0 = Directional)
  STD140(glm::vec4, _position);

  // .xyz = Light Color
  // .w   = Light Intensity (>1.0 for HDR)
  STD140(glm::vec4, _color);

  // .x = Constant Factor
  // .y = Linear Factor
  // .z = Quadratic Factor
  // .w = Radius
  STD140(glm::vec4, _attenuation);

  // .x = Ambient Factor
  // .y = Diffuse Factor
  // .z = Specular Factor
  // .w = Padding (unused)
  STD140(glm::vec4, _factors);
};

static_assert(sizeof(LightData) == 64, "LightData must be exactly 64 bytes");
static_assert(sizeof(LightData) % 16 == 0);

struct alignas(16) LightBlock {
  LightBlock() : _count(0) {}

  LightData _data[kMaxLightsCount];

  STD140(int, _count);
};

static_assert(sizeof(LightBlock) % 16 == 0);
}  // namespace ShaderUtils