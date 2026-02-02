#pragma once

#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

struct aiScene;

namespace Resources {

/**
 * @brief Configuration for model loading operations
 */
struct ModelLoadConfig {
  // Assimp post-processing flags
  unsigned int assimpFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
                             aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph;

  // Validation options
  bool requireNormals = true;
  bool requirePositions = true;
  bool requireTextureCoords = false;  // Optional, some meshes may not have UVs

  // Optimization options
  bool optimizeMeshes = true;
  bool optimizeGraph = true;
  bool generateTangents = true;

  // Logging
  bool verboseLogging = false;
};

/**
 * @brief Helper class for loading and validating 3D models using Assimp
 *
 * Separates concerns of file validation, Assimp import, and scene validation.
 * This class handles the low-level model loading operations and provides
 * a clean interface for the ResourceManager.
 */
class ModelLoader {
 public:
  /**
   * @brief Construct a ModelLoader with default configuration
   *
   * Note: Log level configuration is centralized in main.cpp
   */
  explicit ModelLoader(const ModelLoadConfig &config = ModelLoadConfig{}) : config_(config) {
    // Log level is configured globally in main.cpp, not per-component
  }

  /**
   * @brief Load and validate a model file
   * @param filePath Absolute path to the model file
   * @return Pointer to the imported scene (owned by the importer), or nullptr
   * on failure
   * @throws ModelFileNotFoundException if file doesn't exist
   * @throws ModelImportException if Assimp fails to import
   * @throws ModelCorruptedException if imported scene is invalid
   *
   * Note: The returned scene pointer is owned by the Assimp::Importer instance.
   * The scene will be destroyed when the importer is destroyed.
   */
  const aiScene *loadModel(const std::string &filePath);

  /**
   * @brief Validate that a file path exists and is readable
   */
  static bool validateFilePath(const std::string &filePath) noexcept;

  /**
   * @brief Validate an imported scene for required data
   */
  static void validateScene(const aiScene *scene, const std::string &filePath);

  /**
   * @brief Get the last error message from Assimp
   */
  std::string getLastError() const noexcept { return importer_.GetErrorString(); }

  /**
   * @brief Check if the importer has an error
   */
  bool hasError() const noexcept { return std::string(importer_.GetErrorString()).length() > 0; }

 private:
  Assimp::Importer importer_;
  ModelLoadConfig config_;

  /**
   * @brief Validate Assimp import flags
   */
  void validateImportFlags() const;

  /**
   * @brief Check if file extension is supported
   */
  static bool isSupportedFormat(const std::string &filePath) noexcept;
};

}  // namespace Resources
