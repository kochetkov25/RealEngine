#include "Scene/Scene.h"

#include <algorithm>

#include "Modules/Logger.h"
#include "Scene/Entity.h"

namespace Core {

Entity Scene::createEntity(std::string_view name) {
  const EntityId uid = _nextUid++;
  Entity entity(uid, shared_from_this());

  (void)entity.addComponent<Components::TagComponent>(std::string{name});
  (void)entity.addComponent<Components::TransformComponent>();

  Logger::debug("Scene", "Created entity uid=", uid, " name=\"", name, "\"");
  return entity;
}

void Scene::destroyEntity(EntityId uid) noexcept {
  const auto n = std::max({
      _transformPool.size(),
      _tagPool.size(),
      _meshPool.size(),
      _skeletonPool.size(),
      _animatorPool.size(),
  });
  const auto maxUid = static_cast<EntityId>(n);

  if (uid >= maxUid) {
    Logger::warning("Scene", "destroyEntity: uid ", uid, " out of range (max ", maxUid, ")");
    return;
  }

  if (uid < static_cast<EntityId>(_transformPool.size())) {
    _transformPool[uid].reset();
  }

  if (uid < static_cast<EntityId>(_tagPool.size())) {
    _tagPool[uid].reset();
  }

  if (uid < static_cast<EntityId>(_meshPool.size())) {
    _meshPool[uid].reset();
  }

  if (uid < static_cast<EntityId>(_skeletonPool.size())) {
    _skeletonPool[uid].reset();
  }
  
  if (uid < static_cast<EntityId>(_animatorPool.size())) {
    _animatorPool[uid].reset();
  }

  Logger::debug("Scene", "Destroyed entity uid=", uid);
}

}  // namespace Core
