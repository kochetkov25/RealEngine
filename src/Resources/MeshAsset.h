#pragma once

#include <array>
#include <cstddef>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <vector>

#include "assimp/material.h"

namespace Resources {
class MeshAsset {
 public:
  static constexpr size_t kMaxBonePerVertex = 4;

  struct VertexBoneData {
    VertexBoneData() noexcept : boneIds{}, boneWeights{} {}
    std::array<unsigned int, kMaxBonePerVertex> boneIds;

    std::array<float, kMaxBonePerVertex> boneWeights;
  };

  struct TextureSlot {
    TextureSlot(unsigned int id, aiTextureType type) noexcept : id(id), type(type) {}

    unsigned int id;

    aiTextureType type;
  };

  MeshAsset() = default;
  MeshAsset(const MeshAsset &) = default;
  MeshAsset(MeshAsset &&) noexcept = default;
  MeshAsset &operator=(const MeshAsset &) = default;
  MeshAsset &operator=(MeshAsset &&) noexcept = default;

  ~MeshAsset() noexcept = default;

  // Getters for mesh data
  [[nodiscard]] const std::string &getName() const noexcept { return name_; }

  [[nodiscard]] const std::vector<glm::vec3> &getVertices() const noexcept { return vertices_; }

  [[nodiscard]] const std::vector<glm::vec3> &getNormals() const noexcept { return normals_; }

  [[nodiscard]] const std::vector<glm::vec2> &getTexCoords() const noexcept { return texCoords_; }

  [[nodiscard]] const std::vector<unsigned int> &getIndices() const noexcept { return indices_; }

  [[nodiscard]] const std::vector<TextureSlot> &getTextureSlots() const noexcept { return textures_; }

  [[nodiscard]] const std::optional<std::vector<VertexBoneData>> &getBoneData() const noexcept { return boneData_; }

  [[nodiscard]] bool hasBoneData() const noexcept { return boneData_.has_value(); }

  [[nodiscard]] bool isValid() const noexcept { return !vertices_.empty(); }

  [[nodiscard]] size_t getVertexCount() const noexcept { return vertices_.size(); }

  void setName(const std::string &name) { name_ = name; }
  void setName(std::string &&name) { name_ = std::move(name); }

  void addVertex(const glm::vec3 &vertex) { vertices_.push_back(vertex); }

  void addNormal(const glm::vec3 &normal) { normals_.push_back(normal); }

  void addTexCoord(const glm::vec2 &texCoord) { texCoords_.push_back(texCoord); }

  void addTexture(const TextureSlot &texture) { textures_.push_back(texture); }
  void addTexture(TextureSlot &&texture) { textures_.push_back(std::move(texture)); }

  void reserveVertices(size_t count) { vertices_.reserve(count); }

  void setIndices(std::vector<unsigned int> &&indices) { indices_ = std::move(indices); }

  void addIndex(unsigned int index) { indices_.push_back(index); }

  void setBoneData(std::vector<VertexBoneData> &&boneData) { boneData_ = std::move(boneData); }

  void initializeBoneData(size_t vertexCount) { boneData_ = std::vector<VertexBoneData>(vertexCount); }

  [[nodiscard]] std::optional<std::vector<VertexBoneData>> &getBoneDataMutable() { return boneData_; }

 private:
  std::string name_;
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec2> texCoords_;
  std::vector<unsigned int> indices_;
  std::vector<TextureSlot> textures_;
  std::optional<std::vector<VertexBoneData>> boneData_;
};
}  // namespace Resources
