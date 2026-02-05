#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

#include "Components.h"

namespace Core {
class Entity;

using EntityId = uint32_t;

class Scene : public std::enable_shared_from_this<Scene> {
 public:
  [[nodiscard]] Entity createEntity(std::string_view name);

  void destroyEntity(EntityId uid) noexcept;

  template <typename T>
  [[nodiscard]] std::vector<std::optional<T>>& getPool();
  template <typename T>
  [[nodiscard]] const std::vector<std::optional<T>>& getPool() const;

  template <typename T, typename... Args>
  T& addComponent(EntityId uid, Args&&... args) {
    auto& pool = getPool<T>();
    if (uid >= pool.size()) {
      pool.resize(uid + 1);
    }
    pool[uid].emplace(std::forward<Args>(args)...);
    return *pool[uid];
  }

  template <typename T>
  [[nodiscard]] T& getComponent(EntityId uid) {
    assert(hasComponent<T>(uid) && "Entity does not have this component");
    return *getPool<T>()[uid];
  }

  template <typename T>
  [[nodiscard]] bool hasComponent(EntityId uid) const noexcept {
    const auto& pool = getPool<T>();
    return uid < static_cast<EntityId>(pool.size()) && pool[uid].has_value();
  }

  [[nodiscard]] static std::shared_ptr<Scene> create() { return std::shared_ptr<Scene>(new Scene); }

 private:
  Scene() = default;

  EntityId _nextUid{0};

  std::vector<std::optional<Components::TransformComponent>> _transformPool;
  std::vector<std::optional<Components::TagComponent>> _tagPool;
  std::vector<std::optional<Components::MeshComponent>> _meshPool;
  std::vector<std::optional<Components::SkeletonComponent>> _skeletonPool;
  std::vector<std::optional<Components::AnimatorComponent>> _animatorPool;
};

template <>
inline std::vector<std::optional<Components::TransformComponent>>& Scene::getPool() {
  return _transformPool;
}
template <>
inline const std::vector<std::optional<Components::TransformComponent>>& Scene::getPool() const {
  return _transformPool;
}

template <>
inline std::vector<std::optional<Components::TagComponent>>& Scene::getPool() {
  return _tagPool;
}
template <>
inline const std::vector<std::optional<Components::TagComponent>>& Scene::getPool() const {
  return _tagPool;
}

template <>
inline std::vector<std::optional<Components::MeshComponent>>& Scene::getPool() {
  return _meshPool;
}
template <>
inline const std::vector<std::optional<Components::MeshComponent>>& Scene::getPool() const {
  return _meshPool;
}

template <>
inline std::vector<std::optional<Components::SkeletonComponent>>& Scene::getPool() {
  return _skeletonPool;
}
template <>
inline const std::vector<std::optional<Components::SkeletonComponent>>& Scene::getPool() const {
  return _skeletonPool;
}

template <>
inline std::vector<std::optional<Components::AnimatorComponent>>& Scene::getPool() {
  return _animatorPool;
}
template <>
inline const std::vector<std::optional<Components::AnimatorComponent>>& Scene::getPool() const {
  return _animatorPool;
}
}  // namespace Core

#include "Scene/Entity.h"