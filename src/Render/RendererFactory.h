#pragma once

#include <memory>

#include "Renderer.h"

namespace Render {
class RendererFactory {
 public:
  // minimum for draw 3D models
  static std::shared_ptr<Renderer> CreateDefault3DModelRenderer();

  // minimum for draw skinned 3D models
  static std::shared_ptr<Renderer> CreateSkinned3DModelRenderer();

  // minimum for draw textures
  static std::shared_ptr<Renderer> CreateTextureRenderer();

  // minimum for draw OpenGL primitives
  static std::shared_ptr<Renderer> CreatePrimitivesRenderer();

  // debug
  static std::shared_ptr<Renderer> CreateDebugRenderer();

  // empty
  static std::shared_ptr<Renderer> CreateDebugGridRenderer();
};
}  // namespace Render