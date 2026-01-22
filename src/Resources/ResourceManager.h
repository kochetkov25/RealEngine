#pragma once

#include <assimp/texture.h>

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "ShaderManager.h"

namespace Resources {
class ModelLoader;
class ModelCache;
struct ModelMetadata;
}  // namespace Resources

#include "ModelMesh.h"

namespace Render {
class ShaderProgram;
class Texture2D;
class Sprite;
class AnimatedSprite;
}  // namespace Render

class ResourceManager {
 public:
  // Constructor, takes the path to the executable to resolve relative paths
  ResourceManager();

  // Destructor (needed for unique_ptr with incomplete types)
  ~ResourceManager();

  [[nodiscard]] std::shared_ptr<Render::ShaderProgram> getShaderProgram(
      const Resources::ShaderType type,
      const Resources::ShaderFeatures features = Resources::ShaderFeatures::None) const;

  // Loads a 2D texture from a file
  std::shared_ptr<Render::Texture2D> loadTexture2D(const std::string &textureName, const std::string &texturePath);

  // Load 2D texture from memory (Assimp embedded texture)
  std::shared_ptr<Render::Texture2D> loadTexture2D_memory(const std::string &textureName, const aiTexture *rawData);

  // Retrieves a shared_ptr to a loaded 2D texture
  [[nodiscard]] std::shared_ptr<Render::Texture2D> getTexture2D(const std::string &texture2DName) const;

  // Loads a sprite
  std::shared_ptr<Render::Sprite> loadSprite(const std::string &spriteName, const std::string &textureName,
                                             const std::string &shaderProgramName, const unsigned int spriteWidth,
                                             const unsigned int spriteHeight,
                                             const std::string &subTextureName = "default");

  // Retrieves a shared_ptr to a loaded sprite
  [[nodiscard]] std::shared_ptr<Render::Sprite> getSprite(const std::string &spriteName) const;

  // Loads a texture atlas (2D texture with multiple sub-textures)
  std::shared_ptr<Render::Texture2D> loadTextureAtlas2D(const std::string &texture2DName,
                                                        const std::string &texturePath,
                                                        std::vector<std::string> subTextureNames,
                                                        const unsigned int subTextureWidth,
                                                        const unsigned int subTextureHeight);

  /**
   * @brief Load a 3D model mesh from file
   *
   * This function loads a 3D model using Assimp, with comprehensive error
   * handling, caching, and validation. Models are cached to prevent redundant
   * loading.
   *
   * @param modelName Unique name to identify the loaded model
   * @param modelPath Relative path to the model file (from executable
   * directory)
   * @return Shared pointer to the loaded ModelMesh, or nullptr on failure
   * @throws Resources::ModelLoadException or derived exceptions on failure
   */
  std::shared_ptr<ModelMesh> loadModelMesh(const std::string &modelName, const std::string &modelPath);

  /**
   * @brief Get metadata for a loaded model
   * @param modelName The name of the model
   * @return Optional metadata if model is loaded, nullopt otherwise
   */
  [[nodiscard]] std::optional<Resources::ModelMetadata> getModelMetadata(const std::string &modelName) const noexcept;

  /**
   * @brief Clear the model cache (useful for memory management)
   */
  void clearModelCache() noexcept;

 private:
  // Map for storing 2D textures
  typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> Texture2DMap;
  Texture2DMap _texture2DMaps;

  typedef std::map<const std::string, std::shared_ptr<ModelMesh>> ModelMeshMap;
  ModelMeshMap _modelMeshMaps;

  // Map for storing sprites
  typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpriteMap;
  SpriteMap _spriteMaps;

  // Path to the executable directory
  std::string _path;

  // Model loading infrastructure
  std::unique_ptr<Resources::ModelLoader> _modelLoader;
  std::unique_ptr<Resources::ModelCache> _modelCache;

  std::unique_ptr<Resources::ShaderManager> _shaderManager;

  /**
   * @brief Load embedded textures from an Assimp scene
   * @param scene The imported Assimp scene
   * @return Vector of texture name-texture pairs
   */
  std::vector<std::pair<std::string, std::shared_ptr<Render::Texture2D>>> loadEmbeddedTextures(
      const aiScene *scene) noexcept;
};
