#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "AnimationAsset.h"

namespace Resources {
class SkeletonAsset;


class SkeletonAnimator {
 public:
  SkeletonAnimator(std::shared_ptr<SkeletonAsset> pSkeleton);
  ~SkeletonAnimator() = default;

  void playAnimation(std::shared_ptr<AnimationAsset> pAnimation);

  void update(float deltaTime);

  [[nodiscard]] const std::vector<glm::mat4>& getBoneMatrices() const;

 private:
  glm::vec3 samplePosition(const AnimationChannel& channel, float time);
  glm::quat sampleRotation(const AnimationChannel& channel, float time);
  glm::vec3 sampleScale(const AnimationChannel& channel, float time);

  std::shared_ptr<SkeletonAsset> _skeleton;
  std::shared_ptr<AnimationAsset> _currentAnimation;
  std::vector<glm::mat4> _finalBoneMatrices;

  float _currentTime;
};
}  // namespace Resources