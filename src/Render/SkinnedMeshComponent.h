#pragma once

#include "MeshComponent.h"

namespace Render {
class SkinnedMeshComponent : public MeshComponent {
 public:
  SkinnedMeshComponent(unsigned int meshAssetId) : MeshComponent(meshAssetId) {}
  ~SkinnedMeshComponent() = default;

  void upload(std::shared_ptr<Resources::MeshAsset> meshAsset) override;
  void draw() const override;
};
}  // namespace Render