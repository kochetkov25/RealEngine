#pragma once

#include <cstdint>

namespace Render {
namespace TexSlots {
constexpr uint8_t Diffuse = 0;
constexpr uint8_t Specular = 1;
constexpr uint8_t Normal = 2;
constexpr uint8_t Height = 3;
constexpr uint8_t Ambient = 4;
constexpr uint8_t Emissive = 5;
constexpr uint8_t Metallic = 6;
constexpr uint8_t Roughness = 7;
// Legacy / generic
constexpr uint8_t Generic = 8;

constexpr uint8_t kCount = 9;
}  // namespace TexSlots
}  // namespace Render
