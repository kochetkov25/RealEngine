#include "ResourceManager.h"

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "../Render/ShaderProgram.h"
#include "../Render/Sprite.h"
#include "../Render/Texture2D.h"
#include "ModelCache.h" // Used for cache operations
#include "ModelLoadExceptions.h"
#include "ModelLoader.h"
#include "ModelLogger.h"
#include "ModelMesh.h"
#include "ModelMetadata.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "stb_image.h"

namespace {
const std::string kShaderPath = "res/shaders/";
const std::unordered_set<char> kMustReplace = {'<', '>', '"'};
} // namespace

// Constructor, takes the path to the executable to resolve relative paths
ResourceManager::ResourceManager(const std::string &executablePath) {
  // Extract the directory path from the executable path
  size_t found = executablePath.find_last_of("/\\");
  _path = executablePath.substr(0, found);

  // Set texture flipping once (global setting, don't set per texture)
  stbi_set_flip_vertically_on_load(true);

  // Initialize model loading infrastructure
  Resources::ModelLoadConfig config;
  config.verboseLogging = false; // Set to true for debug builds
  _modelLoader = std::make_unique<Resources::ModelLoader>(config);
  _modelCache = std::make_unique<Resources::ModelCache>();
}

// Destructor
ResourceManager::~ResourceManager() = default;

// Reads a file into a string, resolving path relative to the executable
std::string
ResourceManager::getFileString(const std::string &relativeFilePath) const {
  std::fstream file;
  file.open(_path + "/" + relativeFilePath.c_str(),
            std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed open file (source: " << __FUNCTION__ << ") \n"
              << relativeFilePath << std::endl;
    return std::string();
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string
ResourceManager::resolveShaderIncludes(const std::string &shaderSource) const {
  std::unordered_set<std::string> alreadyIncluded;

  std::ostringstream output;
  std::string line;

  std::istringstream issShader(shaderSource);

  while (std::getline(issShader, line)) {
    auto formatLine = line;
    std::for_each(formatLine.begin(), formatLine.end(), [](auto &ch) {
      if (kMustReplace.count(ch)) {
        ch = ' ';
      }
    });

    std::istringstream issLine(formatLine);
    std::string word;

    issLine >> word;
    if (word != "#include") {
      output << line << '\n';
      continue;
    }

    std::string fileName;
    issLine >> fileName;

    if (alreadyIncluded.count(fileName)) {
      std::cerr << "Duplicate include! File: " << fileName
                << ".(source: " << __FUNCTION__ << ")" << std::endl;
      assert(false);
      continue;
    }
    alreadyIncluded.insert(fileName);

    const auto includeFileSource = getFileString(kShaderPath + fileName);
    output << includeFileSource << '\n';
  }

  return output.str();
}

// Loads a shader program and stores it
std::shared_ptr<Render::ShaderProgram> ResourceManager::loadShaderProgram(
    const std::string &shaderName, const std::string &vertexShaderPathRelative,
    const std::string &fragmentShaderPathRelative) {
  // Load vertex shader source
  std::string vertexString = getFileString(vertexShaderPathRelative);
  vertexString = resolveShaderIncludes(vertexString);
  if (vertexString.empty()) {
    std::cerr << "Failed to load VERTEX SHADER. (source: " << __FUNCTION__
              << ")" << std::endl;
    assert(false);
    return nullptr;
  }

  // Load fragment shader source
  std::string fragmentString = getFileString(fragmentShaderPathRelative);
  fragmentString = resolveShaderIncludes(fragmentString);
  if (fragmentString.empty()) {
    std::cerr << "Failed to load FRAGMENT SHADER. (source: " << __FUNCTION__
              << ")" << std::endl;
    assert(false);
    return nullptr;
  }

  std::shared_ptr<Render::ShaderProgram> pNewShaderProgram =
      std::make_shared<Render::ShaderProgram>(vertexString, fragmentString);
  // Check if shader program compiled successfully
  if (!pNewShaderProgram->isCompiled()) {
    std::cerr << "Can not create new shader program. Path to shaders: \n"
              << "Vertex shader: " << vertexShaderPathRelative << "\n"
              << "Fragment shader: " << fragmentShaderPathRelative << "\n"
              << "(source: " << __FUNCTION__ << ")" << std::endl;
    assert(false);
    return nullptr;
  }
  // Store the new shader program
  auto isShaderProgramAdd =
      _shaderPrograms.emplace(shaderName, pNewShaderProgram);
  return isShaderProgramAdd.first->second;
}

// Retrieves a shared_ptr to a loaded shader program
std::shared_ptr<Render::ShaderProgram>
ResourceManager::getShaderProgram(const std::string &shaderName) const {
  const auto it = _shaderPrograms.find(shaderName);
  if (it != _shaderPrograms.end()) {
    return it->second;
  }
  std::cerr << "Cannot find shader program (source: " << __FUNCTION__ << ") "
            << shaderName << std::endl;
  assert(false);
  return nullptr;
}

// Loads a 2D texture from a file using STBI.
std::shared_ptr<Render::Texture2D>
ResourceManager::loadTexture2D(const std::string &textureName,
                               const std::string &texturePathRelative) {
  int channels = 0;
  int width = 0;
  int height = 0;
  // Flip textures vertically on load to match OpenGL's coordinate system
  stbi_set_flip_vertically_on_load(true);
  // Load texture pixels
  unsigned char *pixelsArr =
      stbi_load(std::string(_path + "/" + texturePathRelative).c_str(), &width,
                &height, &channels, 0);

  if (!pixelsArr) {
    std::cerr << "Cannot load texture image (source: " << __FUNCTION__ << ") "
              << texturePathRelative << std::endl;
    assert(false);
    return nullptr;
  }

  std::shared_ptr<Render::Texture2D> pNewTexture2D =
      std::make_shared<Render::Texture2D>(width, height, pixelsArr, channels,
                                          GL_NEAREST, GL_CLAMP_TO_EDGE);
  auto isNewTexture2DAdd = _texture2DMaps.emplace(textureName, pNewTexture2D);
  // Free image data after creating OpenGL texture
  stbi_image_free(pixelsArr);

  // Return shared_ptr to the new texture
  return isNewTexture2DAdd.first->second;
}

std::shared_ptr<Render::Texture2D>
ResourceManager::loadTexture2D_memory(const std::string &textureName,
                                      const aiTexture *rawData) {

  auto it = _texture2DMaps.find(textureName);
  if (it != _texture2DMaps.end()) {
    return it->second;
  }

  int channels = 0;
  int width = 0;
  int height = 0;
  unsigned char *pixelsArr = nullptr;

  // Flip textures vertically on load to match OpenGL's coordinate system
  stbi_set_flip_vertically_on_load(true);

  // Check if texture is compressed (mHeight == 0) or uncompressed
  if (rawData->mHeight == 0) {
    // Compressed texture - load from memory
    pixelsArr = stbi_load_from_memory(
        reinterpret_cast<const unsigned char *>(rawData->pcData),
        rawData->mWidth, // Size in bytes for compressed data
        &width, &height, &channels, STBI_rgb_alpha);

    // STBI_rgb_alpha always returns 4 channels, regardless of original format
    channels = 4;

  } else {
    // Uncompressed texture - data is already in ARGB8888 format
    width = rawData->mWidth;
    height = rawData->mHeight;
    channels = 4; // aiTexture always uses ARGB8888

    // Convert ARGB to RGBA for OpenGL
    size_t dataSize = width * height * 4;
    pixelsArr = static_cast<unsigned char *>(malloc(dataSize));

    const aiTexel *srcData = rawData->pcData;
    for (size_t i = 0; i < width * height; ++i) {
      pixelsArr[i * 4 + 0] = srcData[i].r;
      pixelsArr[i * 4 + 1] = srcData[i].g;
      pixelsArr[i * 4 + 2] = srcData[i].b;
      pixelsArr[i * 4 + 3] = srcData[i].a;
    }
  }

  if (!pixelsArr) {
    std::cerr << "Cannot load texture: " << textureName
              << " (format hint: " << rawData->achFormatHint << ")"
              << std::endl;
    return nullptr;
  }

  auto pNewTexture2D = std::make_shared<Render::Texture2D>(
      width, height, pixelsArr, channels, GL_NEAREST, GL_CLAMP_TO_EDGE);

  auto result = _texture2DMaps.emplace(textureName, pNewTexture2D);

  if (rawData->mHeight == 0) {
    // Compressed texture - allocated by stb_image
    stbi_image_free(pixelsArr);
  } else {
    // Uncompressed texture - allocated by malloc
    free(pixelsArr);
  }

  return result.first->second;
}

// Retrieves a shared_ptr to a loaded 2D texture
std::shared_ptr<Render::Texture2D>
ResourceManager::getTexture2D(const std::string &texture2DName) const {
  const auto it = _texture2DMaps.find(texture2DName);
  if (it != _texture2DMaps.end()) {
    return it->second;
  }
  std::cerr << "Cannot find texture 2D (source: " << __FUNCTION__ << ") "
            << texture2DName << std::endl;
  assert(false);
  return nullptr;
}

// Loads a sprite
std::shared_ptr<Render::Sprite> ResourceManager::loadSprite(
    const std::string &spriteName, const std::string &textureName,
    const std::string &shaderProgramName, const unsigned int spriteWidth,
    const unsigned int spriteHeight, const std::string &subTextureName) {
  const auto pTexture = getTexture2D(textureName);
  if (!pTexture) {
    std::cerr << "Texture with this name cannot be found (source: "
              << __FUNCTION__ << ") " << textureName << std::endl;
    return nullptr;
  }

  const auto pShaderProgram = getShaderProgram(shaderProgramName);
  if (!pShaderProgram) {
    std::cerr << "Shader program with this name cannot be found (source: "
              << __FUNCTION__ << ") " << shaderProgramName << std::endl;
    assert(false);
    return nullptr;
  }

  std::shared_ptr<Render::Sprite> pNewSprite = std::make_shared<Render::Sprite>(
      pTexture, subTextureName, pShaderProgram, glm::vec3(0.f, 0.f, 0.f),
      glm::vec2(spriteWidth, spriteHeight), 0.f);
  const auto [it, inserted] = _spriteMaps.emplace(spriteName, pNewSprite);
  return it->second;
}

// Retrieves a shared_ptr to a loaded sprite
std::shared_ptr<Render::Sprite>
ResourceManager::getSprite(const std::string &spriteName) const {
  const auto it = _spriteMaps.find(spriteName);
  if (it != _spriteMaps.end()) {
    return it->second;
  }
  std::cerr << "Cannot find sprite (source: " << __FUNCTION__ << ") "
            << spriteName << std::endl;
  assert(false);
  return nullptr;
}

// Loads a texture atlas (2D texture with multiple sub-textures)
std::shared_ptr<Render::Texture2D> ResourceManager::loadTextureAtlas2D(
    const std::string &texture2DName, const std::string &texturePath,
    std::vector<std::string> subTextureNames,
    const unsigned int subTextureWidth, const unsigned int subTextureHeight) {
  // Load the base texture atlas
  auto pTexture = loadTexture2D(texture2DName, texturePath);
  if (pTexture) {
    const unsigned int textureWidth = pTexture->getWidth();
    const unsigned int textureHeight = pTexture->getHeight();
    // Initialize texture offset for sub-textures
    unsigned int currentTextureOffsetX = 0;
    unsigned int currentTextureOffsetY = textureHeight;

    // Margin to prevent bleeding between sub-textures
    const float margin = 0.01f;

    // Iterate through sub-texture names and calculate UV coordinates
    for (auto &currentSubtextureName : subTextureNames) {
      glm::vec2 leftBottomUV(
          static_cast<float>(currentTextureOffsetX + margin) / textureWidth,
          static_cast<float>(currentTextureOffsetY - subTextureHeight +
                             margin) /
              textureHeight);

      glm::vec2 rightTopUV(
          static_cast<float>(currentTextureOffsetX + subTextureWidth - margin) /
              textureWidth,
          static_cast<float>(currentTextureOffsetY - margin) / textureHeight);

      // Add sub-texture to the Texture2D object
      pTexture->addSubTexture2D(currentSubtextureName, leftBottomUV,
                                rightTopUV);

      currentTextureOffsetX += subTextureWidth;
      if (currentTextureOffsetX >= textureWidth) {
        currentTextureOffsetX = 0;
        currentTextureOffsetY -= subTextureHeight;
      }
    }
  }
  return pTexture;
}

std::shared_ptr<ModelMesh>
ResourceManager::loadModelMesh(const std::string &modelName,
                               const std::string &modelPath) {
  try {
    Resources::ModelLogger::info("ResourceManager",
                                 "Loading model: ", modelName,
                                 " from path: ", modelPath);

    const std::string absolutePath = resolvePath(modelPath);

    auto cachedMesh = _modelCache->get(absolutePath);
    if (cachedMesh.has_value()) {
      Resources::ModelLogger::info("ResourceManager",
                                   "Model loaded from cache: ", modelName);
      // Store in name map for lookup by name
      _modelMeshMaps[modelName] = cachedMesh.value();
      return cachedMesh.value();
    }

    const auto existingIt = _modelMeshMaps.find(modelName);
    if (existingIt != _modelMeshMaps.end()) {
      Resources::ModelLogger::warning(
          "ResourceManager", "Model with name '", modelName,
          "' already exists. Returning existing model.");
      return existingIt->second;
    }

    // Load the model using ModelLoader
    // Note: The scene is owned by the importer in ModelLoader, so we must
    // process it immediately before the loader might be reused.
    const aiScene *scene = _modelLoader->loadModel(absolutePath);
    if (!scene) {
      Resources::ModelLogger::error("ResourceManager",
                                    "Failed to load model: ", modelName);
      return nullptr;
    }

    auto metadata =
        Resources::ModelLoader::extractMetadata(scene, absolutePath, modelName);

    auto textures = loadEmbeddedTextures(scene);

    // Create ModelMesh (this processes the scene immediately, so we don't need
    // to keep the scene pointer alive after this)
    auto pNewModelMesh =
        std::make_shared<ModelMesh>(scene, std::move(textures));

    _modelCache->put(absolutePath, pNewModelMesh, metadata);

    const auto [it, inserted] =
        _modelMeshMaps.emplace(modelName, pNewModelMesh);
    if (!inserted) {
      Resources::ModelLogger::warning("ResourceManager",
                                      "Model name collision: ", modelName);
    }

    Resources::ModelLogger::info("ResourceManager",
                                 "Successfully loaded model: ", modelName,
                                 " (Vertices: ", metadata.totalVertices,
                                 ", Meshes: ", metadata.meshCount, ")");

    return it->second;

  } catch (const Resources::ModelFileNotFoundException &e) {
    Resources::ModelLogger::error("ResourceManager",
                                  "Model file not found: ", e.what());
    return nullptr;
  } catch (const Resources::ModelImportException &e) {
    Resources::ModelLogger::error("ResourceManager",
                                  "Model import failed: ", e.what());
    return nullptr;
  } catch (const Resources::ModelCorruptedException &e) {
    Resources::ModelLogger::error("ResourceManager",
                                  "Corrupted model: ", e.what());
    return nullptr;
  } catch (const Resources::TextureLoadException &e) {
    Resources::ModelLogger::error("ResourceManager",
                                  "Texture load failed: ", e.what());
    // Continue with model loading even if textures fail
    // (model may still be usable without textures)
  } catch (const std::exception &e) {
    Resources::ModelLogger::error("ResourceManager",
                                  "Unexpected error loading model: ", e.what());
    return nullptr;
  } catch (...) {
    Resources::ModelLogger::error("ResourceManager",
                                  "Unknown error loading model: ", modelName);
    return nullptr;
  }

  return nullptr;
}

// Loads all predefined shaders
void ResourceManager::loadShaders() {
  // Main shader program
  loadShaderProgram("MainShader", "res/shaders/spriteVertexShader.vert",
                    "res/shaders/spriteFragmentShader.frag");
  // Debug shader program
  loadShaderProgram("DebugShader", "res/shaders/spriteVertexShader.vert",
                    "res/shaders/spriteFragmentShader.frag");
  // Light shader program
  loadShaderProgram("LightShader", "res/shaders/lightVertexShader.vert",
                    "res/shaders/lightFragmentShader.frag");
  // Shader for 3D debug grid
  loadShaderProgram("DebugGridShader", "res/shaders/debuGridVertexShader.vert",
                    "res/shaders/debuGridFragmentShader.frag");
}

std::vector<std::pair<std::string, std::shared_ptr<Render::Texture2D>>>
ResourceManager::loadEmbeddedTextures(const aiScene *scene) noexcept {
  ModelMesh::VecTexGL textures;
  textures.reserve(scene->mNumTextures);

  // Process each embedded texture
  for (unsigned int i = 0; i < scene->mNumTextures; ++i) {
    const aiTexture *aiTex = scene->mTextures[i];
    if (!aiTex) {
      Resources::ModelLogger::warning("ResourceManager",
                                      "Null texture at index: ", i);
      continue;
    }

    // Generate unique texture name
    std::string textureName = aiTex->mFilename.C_Str();
    if (textureName.empty()) {
      textureName = "embedded_texture_" + std::to_string(i);
    }

    try {
      // Check if texture already loaded (check map directly to avoid assertion)
      const auto texIt = _texture2DMaps.find(textureName);
      if (texIt != _texture2DMaps.end()) {
        Resources::ModelLogger::debug(
            "ResourceManager", "Reusing existing texture: ", textureName);
        textures.emplace_back(textureName, texIt->second);
        continue;
      }

      // Load texture from memory
      auto texture = loadTexture2D_memory(textureName, aiTex);

      if (texture) {
        textures.emplace_back(textureName, texture);
        Resources::ModelLogger::debug("ResourceManager",
                                      "Loaded embedded texture: ", textureName);
      } else {
        Resources::ModelLogger::warning(
            "ResourceManager",
            "Failed to load embedded texture: ", textureName);
      }
    } catch (const std::exception &e) {
      Resources::ModelLogger::error("ResourceManager",
                                    "Exception loading texture: ", textureName,
                                    ". Error: ", e.what());
    }
  }

  return textures;
}

std::string
ResourceManager::resolvePath(const std::string &relativePath) const noexcept {
  try {
    // If path is already absolute, return as-is
    if (std::filesystem::path(relativePath).is_absolute()) {
      return relativePath;
    }

    // Otherwise, resolve relative to executable directory
    return (_path + "/" + relativePath);
  } catch (const std::exception &) {
    // Fallback to simple concatenation
    return _path + "/" + relativePath;
  }
}

std::optional<Resources::ModelMetadata>
ResourceManager::getModelMetadata(const std::string &modelName) const noexcept {
  const auto it = _modelMeshMaps.find(modelName);
  if (it != _modelMeshMaps.end()) {
    // Try to find metadata in cache by searching for the model
    // Note: This is a simplified approach. In production, we might want
    // to store metadata alongside the model in the name map.
    return std::nullopt; // Would need to track metadata separately
  }

  return std::nullopt;
}

void ResourceManager::clearModelCache() noexcept {
  _modelCache->clear();
  Resources::ModelLogger::info("ResourceManager", "Model cache cleared");
}
