#include "ResourceManager.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include "../Render/ShaderProgram.h"
#include "../Render/Sprite.h"
#include "../Render/Texture2D.h"
#include "ModelMesh.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "stb_image.h"

namespace {
const std::string kShaderPath = "res/shaders/";
const std::unordered_set<char> kMustReplace = {'<', '>', '"'};
}  // namespace

// Constructor, takes the path to the executable to resolve relative paths
ResourceManager::ResourceManager(const std::string &executablePath) {
  // Extract the directory path from the executable path
  size_t found = executablePath.find_last_of("/\\");
  _path = executablePath.substr(0, found);
}

// Reads a file into a string, resolving path relative to the executable
std::string ResourceManager::getFileString(
    const std::string &relativeFilePath) const {
  std::fstream file;
  file.open(_path + "/" + relativeFilePath.c_str(),
            std::ios::in | std::ios::binary);
  // If file cannot be opened, log an error and return empty string
  if (!file.is_open()) {
    std::cerr << "Failed open file (source: " << __FUNCTION__ << ") \n"
              << relativeFilePath << std::endl;
    return std::string();
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string ResourceManager::resolveShaderIncludes(
    const std::string &shaderSource) const {
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
std::shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(
    const std::string &shaderName) const {
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
std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D(
    const std::string &textureName, const std::string &texturePathRelative) {
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

std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D_memory(
    const std::string &textureName, const aiTexture *rawData) {
  int channels = 0;
  int width = 0;
  int height = 0;
  // Flip textures vertically on load to match OpenGL's coordinate system
  stbi_set_flip_vertically_on_load(true);
  // Load texture pixels from memory
  auto pixelsArr = stbi_loadf_from_memory(
      reinterpret_cast<unsigned char *>(rawData->pcData), rawData->mWidth,
      &width, &height, &channels, STBI_rgb_alpha);

  if (!pixelsArr) {
    std::cerr << "Cannot load texture image (tex name: " << __FUNCTION__
              << ") " << textureName << std::endl;
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

// Retrieves a shared_ptr to a loaded 2D texture
std::shared_ptr<Render::Texture2D> ResourceManager::getTexture2D(
    const std::string &texture2DName) const {
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
std::shared_ptr<Render::Sprite> ResourceManager::getSprite(
    const std::string &spriteName) const {
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

std::shared_ptr<ModelMesh> ResourceManager::loadModelMesh(
    const std::string &modelName, const std::string &modelPath) {
  Assimp::Importer Importer;
  auto pathToModel = _path + "/" + modelPath;
  const auto pScene = Importer.ReadFile(pathToModel.c_str(), ASSIMP_LOAD_FLAGS);
  if (!pScene) {
    std::cerr << "Model with this name cannot be found (source: "
              << __FUNCTION__ << ") " << modelPath << std::endl;
    assert(false);
    return nullptr;
  }

  // Load textures embedded in the model
  std::vector<std::pair<std::string, std::shared_ptr<Render::Texture2D>>> textures;
  // Process each embedded texture
  for (unsigned int i = 0; i < pScene->mNumTextures; i++) {
    textures.push_back(
        std::make_pair<std::string, std::shared_ptr<Render::Texture2D>>(
            pScene->mTextures[i]->mFilename.C_Str(),
            loadTexture2D_memory(pScene->mTextures[i]->mFilename.C_Str(),
                                pScene->mTextures[i])));
  }

  auto pNewModelMesh =
      std::make_shared<ModelMesh>(pScene, std::move(textures));
  const auto [it, inserted] = _modelMeshMaps.emplace(modelName, pNewModelMesh);

  return it->second;
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
