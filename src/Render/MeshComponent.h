#pragma once

#include <memory>

#include "Renderer.h"
#include "Resources/MeshAsset.h"

namespace Render {
class MeshComponent {
 public:
  MeshComponent(unsigned int meshAssetId) : _meshAssetId(meshAssetId) {}
  virtual ~MeshComponent() = default;

  virtual void upload(std::shared_ptr<Resources::MeshAsset> meshAsset) = 0;
  virtual void draw() const = 0;

  [[nodiscard]] unsigned int getMeshAssetId() const noexcept { return _meshAssetId; }

 protected:
  std::shared_ptr<Renderer> _renderer = nullptr;

 private:
  unsigned int _meshAssetId;
};
}  // namespace Render