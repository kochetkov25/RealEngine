#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "Render/ShaderUtils/UniformBone.h"
#include "Render/UniformBuffer.h"
#include "Resources/MeshAsset.h"

namespace Resources {
class AnimationAsset;
class SkeletonAsset;
class TextureAsset;

class SkeletonAnimator;
}  // namespace Resources

namespace Render {
class MeshComponent;
class ShaderProgram;
class Texture2D;

class Model {
 public:
  Model() = default;
  ~Model() = default;

  void addMeshComponent(std::shared_ptr<MeshComponent> component);

  void addMeshAsset(std::shared_ptr<Resources::MeshAsset> asset);
  void addAnimationAsset(std::shared_ptr<Resources::AnimationAsset> asset);

  void setSkeletonAsset(std::shared_ptr<Resources::SkeletonAsset> asset);
  void setSkeletonAnimator(std::shared_ptr<Resources::SkeletonAnimator> animator);

  void setTextureAssets(std::vector<std::shared_ptr<Resources::TextureAsset>> textures);

  void update(float deltaTime);
  void draw(std::shared_ptr<ShaderProgram> shader);

  void setAnimation(std::shared_ptr<Resources::AnimationAsset> pAnimation);

  [[nodiscard]] const std::vector<std::shared_ptr<Resources::AnimationAsset>>& getAnimations() const;

 private:
  uint8_t bindTextures(const std::vector<Resources::MeshAsset::TextureSlot>& slots) const;

  std::vector<std::shared_ptr<MeshComponent>> _meshComponents;

  std::vector<std::shared_ptr<Resources::TextureAsset>> _textureAssets;

  std::vector<std::shared_ptr<Resources::MeshAsset>> _meshAssets;
  std::vector<std::shared_ptr<Resources::AnimationAsset>> _animationAssets;

  std::optional<std::shared_ptr<Resources::SkeletonAsset>> _skeletonAsset;
  std::optional<std::shared_ptr<Resources::SkeletonAnimator>> _skeletonAnimator;

  std::unique_ptr<UniformBuffer<ShaderUtils::BoneBlock>> _boneUBO;

  /*REMOVE! ONLY FOR DEBUG*/
  bool _test_anim = true;
};
}  // namespace Render