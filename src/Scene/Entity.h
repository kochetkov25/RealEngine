#pragma once

#include <cstdint>
#include <memory>

#include "Scene.h"

namespace Core {

class Entity {
 public:
  Entity() = default;

  Entity(uint32_t uid, std::shared_ptr<Scene> scene) noexcept : _uid(uid), _scene(std::move(scene)) {}

  template <typename T, typename... Args>
  [[nodiscard]] T& addComponent(Args&&... args) {
    return _scene->addComponent<T>(_uid, std::forward<Args>(args)...);
  }

  template <typename T>
  [[nodiscard]] T& getComponent() {
    return _scene->getComponent<T>(_uid);
  }

  template <typename T>
  [[nodiscard]] bool hasComponent() const {
    return _scene->hasComponent<T>(_uid);
  }

 private:
  uint32_t _uid{};
  std::shared_ptr<Scene> _scene;
};

}  // namespace Core
