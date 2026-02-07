#pragma once

#include <memory>
#include <vector>

#include "MeshAsset.h"
#include "Resources/AnimationAsset.h"
#include "Resources/SkeletonAsset.h"
#include "Resources/TextureAsset.h"

namespace Resources {
struct ModelData {
  std::vector<std::shared_ptr<Resources::TextureAsset>> textureAssets;

  std::vector<std::shared_ptr<Resources::MeshAsset>> meshAssets;

  std::vector<std::shared_ptr<Resources::AnimationAsset>> animationAssets;

  std::shared_ptr<Resources::SkeletonAsset> skeletonAsset;
};
}  // namespace Resources