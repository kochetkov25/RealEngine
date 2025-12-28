#pragma once

#include <assimp/texture.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#define ASSIMP_LOAD_FLAGS                               \
  (aiProcess_Triangulate |                              \
   aiProcess_GenSmoothNormals /*| aiProcess_FlipUVs*/ | \
   aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace)
class ModelMesh;

namespace Render {
class ShaderProgram;
class Texture2D;
class Sprite;
class AnimatedSprite;
}  // namespace Render

class ResourceManager {
 public:
  // Constructor, takes the path to the executable to resolve relative paths
  ResourceManager(const std::string &executablePath);

  // Loads a shader program and stores it
  std::shared_ptr<Render::ShaderProgram> loadShaderProgram(
      const std::string &shaderName, const std::string &vertexShaderPath,
      const std::string &fragmentShaderPath);

  // Retrieves a shared_ptr to a loaded shader program
  [[nodiscard]] std::shared_ptr<Render::ShaderProgram> getShaderProgram(
      const std::string &shaderName) const;

  // Loads a 2D texture from a file
  std::shared_ptr<Render::Texture2D> loadTexture2D(
      const std::string &textureName, const std::string &texturePath);

  // Load 2D texture from memory (Assimp embedded texture)
  std::shared_ptr<Render::Texture2D> loadTexture2D_memory(
      const std::string &textureName, const aiTexture *rawData);

  // Retrieves a shared_ptr to a loaded 2D texture
  [[nodiscard]] std::shared_ptr<Render::Texture2D> getTexture2D(
      const std::string &texture2DName) const;

  // Loads a sprite
  std::shared_ptr<Render::Sprite> loadSprite(
      const std::string &spriteName, const std::string &textureName,
      const std::string &shaderProgramName, const unsigned int spriteWidth,
      const unsigned int spriteHeight,
      const std::string &subTextureName = "default");

  // Retrieves a shared_ptr to a loaded sprite
  [[nodiscard]] std::shared_ptr<Render::Sprite> getSprite(const std::string &spriteName) const;

  // Loads a texture atlas (2D texture with multiple sub-textures)
  std::shared_ptr<Render::Texture2D> loadTextureAtlas2D(
      const std::string &texture2DName, const std::string &texturePath,
      std::vector<std::string> subTextureNames,
      const unsigned int subTextureWidth, const unsigned int subTextureHeight);

  // Load 3D model mesh
  std::shared_ptr<ModelMesh> loadModelMesh(const std::string &modelName,
                                           const std::string &modelPath);

  // Resource loading utilities
  // Loads all predefined shaders
  void loadShaders();

 private:
  // Reads a file into a string, resolving path relative to the executable
  [[nodiscard]] std::string getFileString(const std::string &relativeFilePath) const;

  // Resolves #include directives within a shader source string
  [[nodiscard]] std::string resolveShaderIncludes(const std::string &shaderSource) const;

  // Map for storing shader programs
  typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>>
      ShaderProgramsMap;
  ShaderProgramsMap _shaderPrograms;

  // Map for storing 2D textures
  typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>>
      Texture2DMap;
  Texture2DMap _texture2DMaps;

  typedef std::map<const std::string, std::shared_ptr<ModelMesh>> ModelMeshMap;
  ModelMeshMap _modelMeshMaps;

  // Map for storing sprites
  typedef std::map<const std::string, std::shared_ptr<Render::Sprite>>
      SpriteMap;
  SpriteMap _spriteMaps;

  // Path to the executable directory
  std::string _path;
};
