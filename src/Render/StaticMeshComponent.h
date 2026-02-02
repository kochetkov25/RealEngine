#pragma once

#include "MeshComponent.h"

namespace Render {
class StaticMeshComponent : public MeshComponent {
 public:
  StaticMeshComponent(unsigned int meshAssetId) : MeshComponent(meshAssetId) {}
  ~StaticMeshComponent() = default;

  void upload(std::shared_ptr<Resources::MeshAsset> meshAsset) override;
  void draw() const override;
};
}  // namespace Render