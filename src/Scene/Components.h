#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Resources/AnimationAsset.h"
#include "Resources/MeshAsset.h"
#include "Resources/SkeletonAsset.h"
#include "Resources/TextureAsset.h"
#include "glm/ext/matrix_transform.hpp"

namespace Components {

struct TransformComponent {
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec3 rotation{0.0f, 0.0f, 0.0f};
  float scale{1.0f};

  [[nodiscard]] glm::mat4 getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3{scale});
    return model;
  }
};

struct TagComponent {
  std::string name;

  explicit TagComponent(std::string n = "") : name(std::move(n)) {}
};

struct MeshComponent {
  std::vector<std::shared_ptr<Resources::MeshAsset>> meshAssets;
  std::vector<std::shared_ptr<Resources::TextureAsset>> textureAssets;
};

struct SkeletonComponent {
  std::shared_ptr<Resources::SkeletonAsset> skeletonAsset;
  std::vector<glm::mat4> finalBoneMatrices;

  void init(std::shared_ptr<Resources::SkeletonAsset> asset) {
    skeletonAsset = asset;
    if (skeletonAsset) {
      const size_t n = skeletonAsset->getBones().size();
      finalBoneMatrices.assign(n, glm::mat4{1.0f});
    }
  }
};

struct AnimatorComponent {
  std::shared_ptr<Resources::AnimationAsset> currentAnimation;
  float currentTime{0.0f};
  float speed{1.0f};
  bool isLooping{true};
};

}  // namespace Components
