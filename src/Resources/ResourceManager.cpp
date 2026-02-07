#include "ResourceManager.h"

#include <assimp/scene.h>

#include <cassert>
#include <cstdlib>
#include <memory>
#include <vector>

#include "FileManager.h"
#include "ModelLoadExceptions.h"
#include "ModelLoader.h"
#include "Modules/Logger.h"
#include "Modules/Random.h"
#include "ParseUtils.h"
#include "Render/MeshComponent.h"
#include "Render/Model.h"
#include "Render/ShaderProgram.h"
#include "Render/SkinnedMeshComponent.h"
#include "Render/Sprite.h"
#include "Render/StaticMeshComponent.h"
#include "Render/Texture2D.h"
#include "SkeletonAnimator.h"
#include "TextureAsset.h"
#include "stb_image.h"

namespace {
struct PixelDeleter {
  bool isCompressed;
  void operator()(unsigned char *ptr) const {
    if (ptr) {
      if (isCompressed)
        stbi_image_free(ptr);
      else
        free(ptr);
    }
  }
};
}  // namespace

// Constructor, takes the path to the executable to resolve relative paths
ResourceManager::ResourceManager() {
  // Set texture flipping once (global setting, don't set per texture)
  stbi_set_flip_vertically_on_load(true);

  // Initialize model loading infrastructure
  Resources::ModelLoadConfig config;
  config.verboseLogging = false;  // Set to true for debug builds
  _modelLoader = std::make_unique<Resources::ModelLoader>(config);

  _shaderManager = std::make_unique<Resources::ShaderManager>();
}

ResourceManager::~ResourceManager() = default;

std::shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(
    const Resources::ShaderType type, const Resources::ShaderFeatures features) const {
  auto ShaderProgram = _shaderManager->getShaderProgram(Resources::ShaderKey{type, features});

  if (!ShaderProgram) {
    Core::Logger::error("ResourceManager", "Shader program not found: ", static_cast<int>(type),
                        " with features: ", static_cast<int>(features));
    assert(false);
    return nullptr;
  }

  return ShaderProgram;
}

// Loads a 2D texture from a file using STBI.
std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D(const std::string &textureName,
                                                                  const std::string &texturePathRelative) {
  int channels = 0;
  int width = 0;
  int height = 0;
  // Flip textures vertically on load to match OpenGL's coordinate system
  stbi_set_flip_vertically_on_load(true);
  // Load texture pixels
  unsigned char *pixelsArr = stbi_load(Resources::FileManager::instance().getAbsolutePath(texturePathRelative).c_str(),
                                       &width, &height, &channels, 0);

  if (!pixelsArr) {
    Core::Logger::error("ResourceManager", "Failed to load texture image: ", texturePathRelative);
    assert(false);
    return nullptr;
  }

  std::shared_ptr<Render::Texture2D> pNewTexture2D =
      std::make_shared<Render::Texture2D>(width, height, pixelsArr, channels, GL_NEAREST, GL_CLAMP_TO_EDGE);
  auto isNewTexture2DAdd = _texture2DMaps.emplace(textureName, pNewTexture2D);
  // Free image data after creating OpenGL texture
  stbi_image_free(pixelsArr);

  // Return shared_ptr to the new texture
  return isNewTexture2DAdd.first->second;
}

std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D_memory(const std::string &textureName,
                                                                         const aiTexture *rawData) {
  auto it = _texture2DMaps.find(textureName);
  if (it != _texture2DMaps.end()) {
    return it->second;
  }

  int width = 0, height = 0, channels = 4;  // aiTexture/stbi GL formats expect 4 channels

  std::unique_ptr<unsigned char, PixelDeleter> pixelsArr(nullptr, {rawData->mHeight == 0});

  if (rawData->mHeight == 0) {
    int actualChannels = 0;
    unsigned char *loadedPixels =
        stbi_load_from_memory(reinterpret_cast<const unsigned char *>(rawData->pcData), rawData->mWidth, &width,
                              &height, &actualChannels, STBI_rgb_alpha);
    pixelsArr.reset(loadedPixels);
  } else {
    width = rawData->mWidth;
    height = rawData->mHeight;

    size_t dataSize = width * height * 4;
    unsigned char *convertedPixels = static_cast<unsigned char *>(malloc(dataSize));

    if (convertedPixels) {
      const aiTexel *srcData = rawData->pcData;

      for (size_t i = 0; i < width * height; ++i) {
        convertedPixels[i * 4 + 0] = srcData[i].r;
        convertedPixels[i * 4 + 1] = srcData[i].g;
        convertedPixels[i * 4 + 2] = srcData[i].b;
        convertedPixels[i * 4 + 3] = srcData[i].a;
      }
    }
    pixelsArr.reset(convertedPixels);
  }

  if (!pixelsArr) {
    Core::Logger::error("ResourceManager", "Failed to load embedded texture: ", textureName,
                        " (format hint: ", rawData->achFormatHint, ")");
    return nullptr;
  }

  auto pNewTexture2D =
      std::make_shared<Render::Texture2D>(width, height, pixelsArr.get(), channels, GL_NEAREST, GL_CLAMP_TO_EDGE);

  auto result = _texture2DMaps.emplace(textureName, pNewTexture2D);
  return result.first->second;
}

// Retrieves a shared_ptr to a loaded 2D texture
std::shared_ptr<Render::Texture2D> ResourceManager::getTexture2D(const std::string &texture2DName) const {
  const auto it = _texture2DMaps.find(texture2DName);
  if (it != _texture2DMaps.end()) {
    return it->second;
  }
  Core::Logger::error("ResourceManager", "Texture 2D not found: ", texture2DName);
  assert(false);
  return nullptr;
}

// TODO: NEED REFACTORING
std::shared_ptr<Render::Sprite> ResourceManager::loadSprite(
    const std::string &spriteName, const std::string &textureName, const std::string &shaderProgramName,
    const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string &subTextureName) {
  // const auto pTexture = getTexture2D(textureName);
  // if (!pTexture) {
  //   Core::Logger::error("ResourceManager", "Texture not found: ", textureName);
  //   return nullptr;
  // }

  // const auto pShaderProgram = getShaderProgram(shaderProgramName);
  // if (!pShaderProgram) {
  //   Core::Logger::error("ResourceManager", "Shader program not found: ", shaderProgramName);
  //   assert(false);
  //   return nullptr;
  // }

  // std::shared_ptr<Render::Sprite> pNewSprite = std::make_shared<Render::Sprite>(
  //     pTexture, subTextureName, pShaderProgram, glm::vec3(0.f, 0.f, 0.f), glm::vec2(spriteWidth, spriteHeight), 0.f);
  // const auto [it, inserted] = _spriteMaps.emplace(spriteName, pNewSprite);
  // return it->second;

  Core::Logger::error("ResourceManager", "loadSprite not implemented");
  assert(false);

  return nullptr;
}

// Retrieves a shared_ptr to a loaded sprite
std::shared_ptr<Render::Sprite> ResourceManager::getSprite(const std::string &spriteName) const {
  const auto it = _spriteMaps.find(spriteName);
  if (it != _spriteMaps.end()) {
    return it->second;
  }
  Core::Logger::error("ResourceManager", "Sprite not found: ", spriteName);
  assert(false);
  return nullptr;
}

// Loads a texture atlas (2D texture with multiple sub-textures)
std::shared_ptr<Render::Texture2D> ResourceManager::loadTextureAtlas2D(const std::string &texture2DName,
                                                                       const std::string &texturePath,
                                                                       std::vector<std::string> subTextureNames,
                                                                       const unsigned int subTextureWidth,
                                                                       const unsigned int subTextureHeight) {
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
      glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + margin) / textureWidth,
                             static_cast<float>(currentTextureOffsetY - subTextureHeight + margin) / textureHeight);

      glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - margin) / textureWidth,
                           static_cast<float>(currentTextureOffsetY - margin) / textureHeight);

      // Add sub-texture to the Texture2D object
      pTexture->addSubTexture2D(currentSubtextureName, leftBottomUV, rightTopUV);

      currentTextureOffsetX += subTextureWidth;
      if (currentTextureOffsetX >= textureWidth) {
        currentTextureOffsetX = 0;
        currentTextureOffsetY -= subTextureHeight;
      }
    }
  }
  return pTexture;
}

std::shared_ptr<Render::Model> ResourceManager::loadModel(const std::string &modelName,
                                                          const std::string &modelRelativePath) {
  try {
    Core::Logger::info("ResourceManager", "Loading model: ", modelName, " from path: ", modelRelativePath);

    const auto absolutePath = Resources::FileManager::instance().getAbsolutePath(modelRelativePath);

    // TODO: load form cache by absolutePath (good)

    const auto existingIt = _modelMaps.find(modelName);
    if (existingIt != _modelMaps.end()) {
      Core::Logger::warning("ResourceManager", "Model with name '", modelName,
                            "' already exists. Returning existing model.");
      return existingIt->second;
    }

    const auto pScene = _modelLoader->loadModel(absolutePath);
    if (!pScene) {
      Core::Logger::error("ResourceManager", "Failed to load model: ", modelName);
      return nullptr;
    }

    auto model = std::make_shared<Render::Model>();
    model->getModelMeta()->_name = modelName;

    model->setTextureAssets(std::move(loadAssimpEmbeddedTextures(pScene)));

    auto skeleton = Resources::parseSkeleton(pScene);
    if (skeleton) {
      model->setSkeletonAsset(skeleton);

      if (pScene->mNumAnimations > 0) {
        for (unsigned int animId = 0; animId < pScene->mNumAnimations; ++animId) {
          auto animationAsset = Resources::parseAnimation(pScene->mAnimations[animId], *skeleton);
          model->addAnimationAsset(animationAsset);
        }

        model->setSkeletonAnimator(std::make_shared<Resources::SkeletonAnimator>(skeleton));
      }
    }

    std::function<void(const aiNode *, const aiScene *)> processNode;
    processNode = [&](const aiNode *pNode, const aiScene *pScene) {
      for (unsigned int meshId = 0; meshId < pNode->mNumMeshes; meshId++) {
        auto mesh = pScene->mMeshes[pNode->mMeshes[meshId]];
        auto material = pScene->mMaterials[mesh->mMaterialIndex];

        auto meshAsset = Resources::parseMesh(mesh, material);

        std::shared_ptr<Render::MeshComponent> meshComponent;
        if (meshAsset->hasBoneData()) {
          meshComponent = std::make_shared<Render::SkinnedMeshComponent>(meshId);
        } else {
          meshComponent = std::make_shared<Render::StaticMeshComponent>(meshId);
        }
        meshComponent->upload(meshAsset);

        model->addMeshComponent(meshComponent);
        model->addMeshAsset(meshAsset);
      }

      for (unsigned int childId = 0; childId < pNode->mNumChildren; childId++) {
        processNode(pNode->mChildren[childId], pScene);
      }
    };
    processNode(pScene->mRootNode, pScene);

    const auto [it, inserted] = _modelMaps.emplace(modelName, model);
    if (!inserted) {
      Core::Logger::warning("ResourceManager", "Model name collision: ", modelName);
    }

    Core::Logger::info("ResourceManager", "Successfully loaded model: ", modelName);

    return it->second;
  } catch (const Resources::ModelFileNotFoundException &e) {
    Core::Logger::error("ResourceManager", "Model file not found: ", e.what());
    return nullptr;
  } catch (const Resources::ModelImportException &e) {
    Core::Logger::error("ResourceManager", "Model import failed: ", e.what());
    return nullptr;
  } catch (const Resources::ModelCorruptedException &e) {
    Core::Logger::error("ResourceManager", "Corrupted model: ", e.what());
    return nullptr;
  } catch (const Resources::TextureLoadException &e) {
    Core::Logger::error("ResourceManager", "Texture load failed: ", e.what());
    // Continue with model loading even if textures fail
    // (model may still be usable without textures)
  } catch (const std::exception &e) {
    Core::Logger::error("ResourceManager", "Unexpected error loading model: ", e.what());
    return nullptr;
  } catch (...) {
    Core::Logger::error("ResourceManager", "Unknown error loading model: ", modelName);
    return nullptr;
  }

  return nullptr;
}

std::shared_ptr<Resources::ModelData> ResourceManager::loadModelData(const std::string &modelRelativePath) {
  try {
    Core::Logger::info("ResourceManager", "Loading model: from path: ", modelRelativePath);

    const auto absolutePath = Resources::FileManager::instance().getAbsolutePath(modelRelativePath);

    const auto existingIt = _modelDatas.find(absolutePath);
    if (existingIt != _modelDatas.end()) {
      Core::Logger::warning("ResourceManager", "Model form ", modelRelativePath,
                            "' already exists. Returning existing model.");
      return existingIt->second;
    }

    const auto pScene = _modelLoader->loadModel(absolutePath);
    if (!pScene) {
      Core::Logger::error("ResourceManager", "Failed to load from ", modelRelativePath);
      return nullptr;
    }

    auto modelData = std::make_shared<Resources::ModelData>();

    modelData->textureAssets = Resources::parseEmbeddedTextures(pScene);

    auto skeleton = Resources::parseSkeleton(pScene);
    if (skeleton) {
      modelData->skeletonAsset = skeleton;

      if (pScene->mNumAnimations > 0) {
        for (unsigned int animId = 0; animId < pScene->mNumAnimations; ++animId) {
          modelData->animationAssets.push_back(Resources::parseAnimation(pScene->mAnimations[animId], *skeleton));
        }
      }
    }

    std::function<void(const aiNode *, const aiScene *)> processNode;
    processNode = [&](const aiNode *pNode, const aiScene *pScene) {
      for (unsigned int meshId = 0; meshId < pNode->mNumMeshes; meshId++) {
        auto mesh = pScene->mMeshes[pNode->mMeshes[meshId]];
        auto material = pScene->mMaterials[mesh->mMaterialIndex];

        modelData->meshAssets.push_back(Resources::parseMesh(mesh, material));
      }

      for (unsigned int childId = 0; childId < pNode->mNumChildren; childId++) {
        processNode(pNode->mChildren[childId], pScene);
      }
    };
    processNode(pScene->mRootNode, pScene);

    const auto [it, inserted] = _modelDatas.emplace(modelRelativePath, modelData);
    if (!inserted) {
      Core::Logger::warning("ResourceManager", "Model relativePath collision: ", modelRelativePath);
    }

    Core::Logger::info("ResourceManager", "Successfully loaded model from: ", modelRelativePath);

    return it->second;
  } catch (const Resources::ModelFileNotFoundException &e) {
    Core::Logger::error("ResourceManager", "Model file not found: ", e.what());
    return nullptr;
  } catch (const Resources::ModelImportException &e) {
    Core::Logger::error("ResourceManager", "Model import failed: ", e.what());
    return nullptr;
  } catch (const Resources::ModelCorruptedException &e) {
    Core::Logger::error("ResourceManager", "Corrupted model: ", e.what());
    return nullptr;
  } catch (const Resources::TextureLoadException &e) {
    Core::Logger::error("ResourceManager", "Texture load failed: ", e.what());
  } catch (const std::exception &e) {
    Core::Logger::error("ResourceManager", "Unexpected error loading model: ", e.what());
    return nullptr;
  } catch (...) {
    Core::Logger::error("ResourceManager", "Unknown error loading model from: ", modelRelativePath);
    return nullptr;
  }
  return nullptr;
}

std::vector<std::shared_ptr<Resources::TextureAsset>> ResourceManager::loadAssimpEmbeddedTextures(
    const aiScene *scene) noexcept {
  std::vector<std::shared_ptr<Resources::TextureAsset>> textures;
  textures.reserve(scene->mNumTextures);

  for (unsigned int i = 0; i < scene->mNumTextures; ++i) {
    const aiTexture *aiTex = scene->mTextures[i];
    if (!aiTex) {
      Core::Logger::warning("ResourceManager", "Null texture at index: ", i);
      continue;
    }

    // Generate unique texture name
    std::string textureName = aiTex->mFilename.C_Str();
    textureName += "_embed_tex_" + Core::Random::generate();

    try {
      auto texture = loadTexture2D_memory(textureName, aiTex);

      if (texture) {
        textures.emplace_back(std::make_shared<Resources::TextureAsset>(textureName, texture));
        Core::Logger::debug("ResourceManager", "Loaded embedded texture: ", textureName,
                            " size: ", texture->getHeight(), " x ", texture->getWidth());
      } else {
        Core::Logger::warning("ResourceManager", "Failed to load embedded texture: ", textureName);
      }
    } catch (const std::exception &e) {
      Core::Logger::error("ResourceManager", "Exception loading texture: ", textureName, ". Error: ", e.what());
    }
  }

  return textures;
}
