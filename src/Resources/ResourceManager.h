#pragma once

#include <assimp/texture.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ShaderManager.h"

struct aiScene;

namespace Resources {
class ModelLoader;
struct TextureAsset;
}  // namespace Resources

namespace Render {
class ShaderProgram;
class Texture2D;
class Sprite;
class AnimatedSprite;
class Model;
}  // namespace Render

class ResourceManager {
 public:
  ResourceManager();

  ~ResourceManager();

  [[nodiscard]] std::shared_ptr<Render::ShaderProgram> getShaderProgram(
      const Resources::ShaderType type,
      const Resources::ShaderFeatures features = Resources::ShaderFeatures::None) const;

  // Loads a 2D texture from a file
  [[nodiscard]] std::shared_ptr<Render::Texture2D> loadTexture2D(const std::string &textureName,
                                                                 const std::string &texturePath);

  // Load 2D texture from memory (Assimp embedded texture)
  [[nodiscard]] std::shared_ptr<Render::Texture2D> loadTexture2D_memory(const std::string &textureName,
                                                                        const aiTexture *rawData);

  [[nodiscard]] std::shared_ptr<Render::Texture2D> getTexture2D(const std::string &texture2DName) const;

  [[nodiscard]] std::shared_ptr<Render::Sprite> loadSprite(
      const std::string &spriteName, const std::string &textureName, const std::string &shaderProgramName,
      const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string &subTextureName = "default");

  [[nodiscard]] std::shared_ptr<Render::Sprite> getSprite(const std::string &spriteName) const;

  // Loads a texture atlas (2D texture with multiple sub-textures)
  [[nodiscard]] std::shared_ptr<Render::Texture2D> loadTextureAtlas2D(const std::string &texture2DName,
                                                                      const std::string &texturePath,
                                                                      std::vector<std::string> subTextureNames,
                                                                      const unsigned int subTextureWidth,
                                                                      const unsigned int subTextureHeight);

  [[nodiscard]] std::shared_ptr<Render::Model> loadModel(const std::string &modelName,
                                                         const std::string &modelRelativePath);

 private:
  std::vector<std::shared_ptr<Resources::TextureAsset>> loadAssimpEmbeddedTextures(const aiScene *scene) noexcept;

  typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> Texture2DMap;
  Texture2DMap _texture2DMaps;

  typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpriteMap;
  SpriteMap _spriteMaps;

  typedef std::map<const std::string, std::shared_ptr<Render::Model>> ModelMap;
  ModelMap _modelMaps;

  std::unique_ptr<Resources::ModelLoader> _modelLoader;

  std::unique_ptr<Resources::ShaderManager> _shaderManager;
};
