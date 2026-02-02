#pragma once

#include <memory>
#include <string>

#include "Render/Texture2D.h"

namespace Resources {
struct TextureAsset {
  TextureAsset(const std::string& name, std::shared_ptr<Render::Texture2D> texture) noexcept
      : name(name), texture(texture) {}

  std::string name;
  std::shared_ptr<Render::Texture2D> texture;
};
}  // namespace Resources