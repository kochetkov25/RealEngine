#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>

namespace Render {
class ShaderProgram;
}

namespace Resources {
enum class ShaderFeatures : uint32_t {
  None = 0,
  Skinned = 1 << 0,
};

inline ShaderFeatures operator|(ShaderFeatures a, ShaderFeatures b) {
  return static_cast<ShaderFeatures>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline ShaderFeatures operator&(ShaderFeatures a, ShaderFeatures b) {
  return static_cast<ShaderFeatures>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
inline ShaderFeatures operator|=(ShaderFeatures &a, ShaderFeatures b) {
  a = a | b;
  return a;
}

inline bool HasFlag(ShaderFeatures mask, ShaderFeatures flag) {
  return (static_cast<uint32_t>(mask) & static_cast<uint32_t>(flag)) != 0;
}

enum class ShaderType {
  DebugGrid,
  Light,
  Sprite2D,
  Mesh,
};

struct ShaderKey {
  ShaderType type;
  ShaderFeatures features;

  bool operator==(const ShaderKey &other) const { return type == other.type && features == other.features; }
};

struct ShaderKeyHash {
  size_t operator()(const ShaderKey &key) const {
    size_t h1 = std::hash<int>{}(static_cast<int>(key.type));
    size_t h2 = std::hash<int>{}(static_cast<int>(key.features));

    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

class ShaderManager {
 public:
  ShaderManager() = default;
  ~ShaderManager() = default;

  [[nodiscard]] std::shared_ptr<Render::ShaderProgram> getShaderProgram(const ShaderKey &key);

 private:
  [[nodiscard]] std::shared_ptr<Render::ShaderProgram> createShaderProgram(const ShaderKey &key);

  std::unordered_map<ShaderKey, std::shared_ptr<Render::ShaderProgram>, ShaderKeyHash> _shaderPrograms;
};
}  // namespace Resources