#include "SkeletonAnimator.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "SkeletonAsset.h"

namespace Resources {
SkeletonAnimator::SkeletonAnimator(std::shared_ptr<SkeletonAsset> pSkeleton) : _skeleton(pSkeleton), _currentTime(.0f) {
  _finalBoneMatrices.resize(_skeleton->getBones().size(), glm::mat4(1.0f));
}

void SkeletonAnimator::playAnimation(std::shared_ptr<AnimationAsset> pAnimation) {
  _currentAnimation = pAnimation;
  _currentTime = 0.0f;
}

void SkeletonAnimator::update(float deltaTime) {
  if (!_currentAnimation || !_skeleton) {
    return;
  }

  /*First step*/
  _currentTime += deltaTime;
  _currentTime = fmod(_currentTime, _currentAnimation->durationInSeconds);

  const auto& bones = _skeleton->getBones();
  size_t boneCount = bones.size();

  /*TODO: optimize (save matrices in class)*/
  std::vector<glm::mat4> localTransforms(boneCount);
  std::vector<glm::mat4> globalTransforms(boneCount);

  /*Second step*/
  for (size_t i = 0; i < boneCount; ++i) {
    localTransforms[i] = bones[i].restTransform;  // default pose
  }

  /*sampling*/
  for (const auto& channel : _currentAnimation->channels) {
    unsigned int boneId = channel.boneId;

    glm::vec3 position = samplePosition(channel, _currentTime);
    glm::quat rotation = sampleRotation(channel, _currentTime);
    glm::vec3 scale = sampleScale(channel, _currentTime);

    localTransforms[boneId] =
        glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
  }

  /*Third step*/
  for (size_t i = 0; i < boneCount; ++i) {
    const auto& bone = bones[i];

    if (bone.parentId.has_value()) {
      globalTransforms[i] = globalTransforms[bone.parentId.value()] * localTransforms[i];
    } else {  // Root bone
      globalTransforms[i] = localTransforms[i];
    }

    _finalBoneMatrices[i] = _skeleton->getGlobalInverseTransform() * globalTransforms[i] * bone.offsetMatrix;
  }
}

const std::vector<glm::mat4>& SkeletonAnimator::getBoneMatrices() const { return _finalBoneMatrices; }

glm::vec3 SkeletonAnimator::samplePosition(const AnimationChannel& channel, float time) {
  const auto& keys = channel.positionKeys;
  if (keys.empty()) {
    return glm::vec3(0.0f);
  }
  if (keys.size() == 1) {
    return keys[0].value;
  }

  auto nextIt =
      std::lower_bound(keys.begin(), keys.end(), time, [](const auto& key, float t) { return key.timestamp < t; });

  if (nextIt == keys.begin()) {
    return nextIt->value;
  }
  if (nextIt == keys.end()) {
    return keys.back().value;
  }

  auto prevIt = nextIt - 1;

  float factor = (time - prevIt->timestamp) / (nextIt->timestamp - prevIt->timestamp);
  return glm::mix(prevIt->value, nextIt->value, factor);
}

glm::quat SkeletonAnimator::sampleRotation(const AnimationChannel& channel, float time) {
  const auto& keys = channel.rotationKeys;
  if (keys.empty()) {
    return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  }
  if (keys.size() == 1) {
    return keys[0].value;
  }

  auto nextIt =
      std::lower_bound(keys.begin(), keys.end(), time, [](const auto& key, float t) { return key.timestamp < t; });

  if (nextIt == keys.begin()) {
    return nextIt->value;
  }
  if (nextIt == keys.end()) {
    return keys.back().value;
  }

  auto prevIt = nextIt - 1;

  float factor = (time - prevIt->timestamp) / (nextIt->timestamp - prevIt->timestamp);
  return glm::normalize(glm::slerp(prevIt->value, nextIt->value, factor));
}

glm::vec3 SkeletonAnimator::sampleScale(const AnimationChannel& channel, float time) {
  /*TODO*/
  return glm::vec3(1.0f);
}
}  // namespace Resources