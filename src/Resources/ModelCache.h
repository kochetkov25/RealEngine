#pragma once

#include "ModelMesh.h"
#include "ModelMetadata.h"
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace Resources {

/**
 * @brief Thread-safe cache for loaded 3D models
 *
 * Prevents redundant loading of the same model file and provides
 * efficient lookup. Uses weak_ptr to allow models to be unloaded
 * when no longer referenced.
 */
class ModelCache {
public:
  /**
   * @brief Cache entry structure
   */
  struct CacheEntry {
    std::weak_ptr<ModelMesh> mesh;
    ModelMetadata metadata;
    std::string filePath;
    std::chrono::system_clock::time_point loadTime;

    CacheEntry(std::shared_ptr<ModelMesh> m, const ModelMetadata &meta,
               const std::string &path)
        : mesh(m), metadata(meta), filePath(path),
          loadTime(std::chrono::system_clock::now()) {}
  };

  /**
   * @brief Try to get a cached model
   * @param filePath The file path used as cache key
   * @return Shared pointer to cached model if found and still valid, nullopt
   * otherwise
   */
  std::optional<std::shared_ptr<ModelMesh>>
  get(const std::string &filePath) noexcept;

  /**
   * @brief Store a model in the cache
   * @param filePath The file path used as cache key
   * @param mesh Shared pointer to the model mesh
   * @param metadata Metadata associated with the model
   */
  void put(const std::string &filePath, std::shared_ptr<ModelMesh> mesh,
           const ModelMetadata &metadata) noexcept;

  /**
   * @brief Remove a specific entry from the cache
   */
  void remove(const std::string &filePath) noexcept;

  /**
   * @brief Clear all expired entries (where weak_ptr is expired)
   * @return Number of entries removed
   */
  size_t cleanupExpired() noexcept;

  /**
   * @brief Clear all cache entries
   */
  void clear() noexcept;

  /**
   * @brief Get cache statistics
   */
  struct CacheStats {
    size_t totalEntries;
    size_t validEntries;
    size_t expiredEntries;
  };
  CacheStats getStats() const noexcept;

  /**
   * @brief Get metadata for a cached model
   */
  std::optional<ModelMetadata>
  getMetadata(const std::string &filePath) const noexcept;

private:
  mutable std::mutex mutex_;
  std::unordered_map<std::string, CacheEntry> cache_;

  /**
   * @brief Normalize file path for use as cache key
   */
  static std::string normalizePath(const std::string &path) noexcept;
};

} // namespace Resources
