#include "ModelCache.h"
#include <filesystem>

namespace Resources {

std::optional<std::shared_ptr<ModelMesh>>
ModelCache::get(const std::string &filePath) noexcept {
  std::lock_guard<std::mutex> lock(mutex_);
  const std::string normalizedPath = normalizePath(filePath);

  auto it = cache_.find(normalizedPath);
  if (it == cache_.end()) {
    return std::nullopt;
  }

  // Try to lock the weak_ptr
  auto mesh = it->second.mesh.lock();
  if (!mesh) {
    // Entry expired, remove it
    cache_.erase(it);
    return std::nullopt;
  }

  return mesh;
}

void ModelCache::put(const std::string &filePath,
                     std::shared_ptr<ModelMesh> mesh,
                     const ModelMetadata &metadata) noexcept {
  if (!mesh) {
    return;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  const std::string normalizedPath = normalizePath(filePath);

  cache_.emplace(normalizedPath, CacheEntry(mesh, metadata, normalizedPath));
}

void ModelCache::remove(const std::string &filePath) noexcept {
  std::lock_guard<std::mutex> lock(mutex_);
  const std::string normalizedPath = normalizePath(filePath);
  cache_.erase(normalizedPath);
}

size_t ModelCache::cleanupExpired() noexcept {
  std::lock_guard<std::mutex> lock(mutex_);

  size_t removed = 0;
  auto it = cache_.begin();
  while (it != cache_.end()) {
    if (it->second.mesh.expired()) {
      it = cache_.erase(it);
      ++removed;
    } else {
      ++it;
    }
  }

  return removed;
}

void ModelCache::clear() noexcept {
  std::lock_guard<std::mutex> lock(mutex_);
  cache_.clear();
}

ModelCache::CacheStats ModelCache::getStats() const noexcept {
  std::lock_guard<std::mutex> lock(mutex_);

  CacheStats stats{};
  stats.totalEntries = cache_.size();

  for (const auto &[key, entry] : cache_) {
    if (entry.mesh.expired()) {
      stats.expiredEntries++;
    } else {
      stats.validEntries++;
    }
  }

  return stats;
}

std::optional<ModelMetadata>
ModelCache::getMetadata(const std::string &filePath) const noexcept {
  std::lock_guard<std::mutex> lock(mutex_);
  const std::string normalizedPath = normalizePath(filePath);

  auto it = cache_.find(normalizedPath);
  if (it == cache_.end()) {
    return std::nullopt;
  }

  return it->second.metadata;
}

std::string ModelCache::normalizePath(const std::string &path) noexcept {
  try {
    // Convert to absolute path and normalize
    std::filesystem::path normalized = std::filesystem::absolute(path);
    normalized = normalized.lexically_normal();
    return normalized.string();
  } catch (const std::exception &) {
    // If path normalization fails, return original path
    return path;
  }
}

} // namespace Resources
