#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Resources {
class SkeletonAsset {
 public:
  struct Bone {
    std::string name;
    unsigned int id;
    std::optional<unsigned int> parentId;
    glm::mat4 offsetMatrix{1.0f};
    glm::mat4 restTransform{1.0f};
  };

  SkeletonAsset() = default;
  ~SkeletonAsset() = default;

  [[nodiscard]] const std::vector<Bone> &getBones() const noexcept { return _bones; }
  [[nodiscard]] std::vector<Bone> &getBones() noexcept { return _bones; }

  [[nodiscard]] const std::unordered_map<std::string, unsigned int> &getBoneNameToId() const noexcept {
    return _boneNameToId;
  }
  [[nodiscard]] const glm::mat4 &getGlobalInverseTransform() const noexcept { return _globalInverseTransform; }

  void setBones(std::vector<Bone> &&bones) { _bones = std::move(bones); }
  void setBoneNameToId(std::unordered_map<std::string, unsigned int> &&boneNameToId) {
    _boneNameToId = std::move(boneNameToId);
  }
  void setGlobalInverseTransform(const glm::mat4 &globalInverseTransform) {
    _globalInverseTransform = globalInverseTransform;
  }

 private:
  std::vector<Bone> _bones;
  std::unordered_map<std::string, unsigned int> _boneNameToId;
  glm::mat4 _globalInverseTransform{1.0f};
};
}  // namespace Resources