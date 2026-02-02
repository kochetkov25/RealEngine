#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

namespace Resources {
struct VectorKey {
  float timestamp;
  glm::vec3 value;
};

struct QuatKey {
  float timestamp;
  glm::quat value;
};

struct AnimationChannel {
  int boneId;
  std::vector<VectorKey> positionKeys;
  std::vector<QuatKey> rotationKeys;
  std::vector<VectorKey> scaleKeys;
};

class AnimationAsset {
 public:
  std::string name;
  float durationInSeconds;
  std::vector<AnimationChannel> channels;
};
}  // namespace Resources