#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "BaseMesh.h"

struct aiScene;

namespace Render {
class Texture2D;
class ShaderProgram;
} // namespace Render

/**
 * @brief Represents a complete 3D model composed of multiple meshes
 *
 * Handles rendering of complex 3D models with multiple meshes, materials,
 * and textures. Optimized for efficient GPU state management and rendering.
 */
class ModelMesh {
public:
  typedef std::vector<
      std::pair<std::string, std::shared_ptr<Render::Texture2D>>>
      VecTexGL;

  /**
   * @brief Construct a ModelMesh from an Assimp scene
   * @param pScene The imported Assimp scene
   * @param vecTexGL Vector of texture name-texture pairs (embedded textures)
   */
  ModelMesh(const aiScene *pScene, VecTexGL vecTexGL);

  /**
   * @brief Draw the model with optimized rendering
   *
   * Renders all meshes in the model with efficient GPU state management.
   * Minimizes redundant texture binds and shader uniform updates.
   *
   * @param shader Shader program to use for rendering (must not be null)
   * @param skipEmptyMeshes If true, skips meshes with no geometry (default:
   * true)
   */
  void draw(std::shared_ptr<Render::ShaderProgram> shader,
            bool skipEmptyMeshes = true) const;

  /**
   * @brief Get the number of meshes in this model
   */
  [[nodiscard]] size_t getMeshCount() const noexcept {
    return _vecMeshes.size();
  }

  /**
   * @brief Check if the model has any meshes
   */
  [[nodiscard]] bool isEmpty() const noexcept { return _vecMeshes.empty(); }

private:
  /**
   * @brief Process a node in the Assimp scene graph
   */
  void processNode(const aiNode *pNode, const aiScene *pScene);

  /**
   * @brief Bind textures for a mesh efficiently
   * @param meshTextures Textures associated with the mesh
   * @param shader Shader program to set uniforms on
   * @return Number of textures bound
   */
  uint8_t bindMeshTextures(const std::vector<BaseMesh::Texture> &meshTextures,
                           std::shared_ptr<Render::ShaderProgram> shader) const;

  std::vector<std::shared_ptr<BaseMesh>> _vecMeshes;
  VecTexGL _vecTexGL;
};