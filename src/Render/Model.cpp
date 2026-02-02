#include "Model.h"

#include <algorithm>
#include <optional>

#include "Modules/Logger.h"
#include "Resources/SkeletonAnimator.h"
#include "Resources/SkeletonAsset.h"
#include "Resources/TextureAsset.h"
#include "ShaderProgram.h"
#include "StaticMeshComponent.h"
#include "TexSlots.h"

namespace {
void resetMaterialState(std::shared_ptr<Render::ShaderProgram> shader) {
  shader->setBoolUniform("hasMaterialDiffuse", false);
  shader->setBoolUniform("hasMaterialSpecular", false);
  shader->setBoolUniform("hasMaterialNormal", false);
  shader->setBoolUniform("hasMaterialHeight", false);
  shader->setBoolUniform("hasMaterialAmbient", false);
  shader->setBoolUniform("hasMaterialEmissive", false);
  shader->setBoolUniform("hasMaterialMetallic", false);
  shader->setBoolUniform("hasMaterialRoughness", false);
  shader->setBoolUniform("hasMaterialTexture", false);
  shader->setBoolUniform("hasMaterialSpecularMap", false);
  shader->setBoolUniform("hasMaterialEmissionMap", false);
}

void setMaterialFlagsFromSlots(std::shared_ptr<Render::ShaderProgram> shader,
                               const std::vector<Resources::MeshAsset::TextureSlot> &slots) {
  if (!shader) {
    return;
  }

  for (const auto &slot : slots) {
    switch (slot.type) {
      case aiTextureType_DIFFUSE:
      case aiTextureType_BASE_COLOR:
        shader->setBoolUniform("hasMaterialDiffuse", true);
        shader->setBoolUniform("hasMaterialTexture", true);
        break;
      case aiTextureType_SPECULAR:
        shader->setBoolUniform("hasMaterialSpecular", true);
        shader->setBoolUniform("hasMaterialSpecularMap", true);
        break;
      case aiTextureType_NORMALS:
      case aiTextureType_NORMAL_CAMERA:
        shader->setBoolUniform("hasMaterialNormal", true);
        shader->setBoolUniform("useNormalMapping", true);
        break;
      case aiTextureType_HEIGHT:
      case aiTextureType_DISPLACEMENT:
        shader->setBoolUniform("hasMaterialHeight", true);
        break;
      case aiTextureType_AMBIENT:
      case aiTextureType_AMBIENT_OCCLUSION:
        shader->setBoolUniform("hasMaterialAmbient", true);
        break;
      case aiTextureType_EMISSIVE:
        shader->setBoolUniform("hasMaterialEmissive", true);
        shader->setBoolUniform("hasMaterialEmissionMap", true);
        break;
      case aiTextureType_METALNESS:
        shader->setBoolUniform("hasMaterialMetallic", true);
        break;
      case aiTextureType_DIFFUSE_ROUGHNESS:
        shader->setBoolUniform("hasMaterialRoughness", true);
        break;
      default:
        shader->setBoolUniform("hasMaterialTexture", true);
        break;
    }
  }
}

uint8_t textureUnitForType(aiTextureType type) {
  using namespace Render::TexSlots;
  switch (type) {
    case aiTextureType_DIFFUSE:
    case aiTextureType_BASE_COLOR:
      return Diffuse;
    case aiTextureType_SPECULAR:
      return Specular;
    case aiTextureType_NORMALS:
    case aiTextureType_NORMAL_CAMERA:
      return Normal;
    case aiTextureType_HEIGHT:
    case aiTextureType_DISPLACEMENT:
      return Height;
    case aiTextureType_AMBIENT:
    case aiTextureType_AMBIENT_OCCLUSION:
      return Ambient;
    case aiTextureType_EMISSIVE:
      return Emissive;
    case aiTextureType_METALNESS:
      return Metallic;
    case aiTextureType_DIFFUSE_ROUGHNESS:
      return Roughness;
    default:
      return Generic;
  }
}
}  // namespace

namespace Render {
void Model::addMeshComponent(std::shared_ptr<MeshComponent> component) { _meshComponents.push_back(component); }

void Model::addMeshAsset(std::shared_ptr<Resources::MeshAsset> asset) { _meshAssets.push_back(asset); }

void Model::addAnimationAsset(std::shared_ptr<Resources::AnimationAsset> asset) { _animationAssets.push_back(asset); }

void Model::setSkeletonAsset(std::shared_ptr<Resources::SkeletonAsset> asset) {
  if (_skeletonAsset.has_value()) {
    Core::Logger::error("Model", "Skeleton asset already set. Will be overwritten.");
  }
  _skeletonAsset = std::make_optional(asset);

  if (!_boneUBO) {
    _boneUBO = std::make_unique<UniformBuffer<ShaderUtils::BoneBlock>>(2);
  }
}

void Model::setSkeletonAnimator(std::shared_ptr<Resources::SkeletonAnimator> animator) {
  if (_skeletonAnimator.has_value()) {
    Core::Logger::error("Model", "Skeleton animator already set. Will be overwritten.");
  }
  _skeletonAnimator = std::make_optional(animator);
}

void Model::setTextureAssets(std::vector<std::shared_ptr<Resources::TextureAsset>> textures) {
  _textureAssets = textures;
}

void Model::update(float deltaTime) {
  if (_skeletonAnimator.has_value()) {
    _skeletonAnimator.value()->update(deltaTime);

    if (_boneUBO) {
      const auto &boneMatrices = _skeletonAnimator.value()->getBoneMatrices();

      ShaderUtils::BoneBlock boneBlock;
      auto boneCount = std::min(boneMatrices.size(), static_cast<size_t>(ShaderUtils::kMaxBones));
      std::copy(boneMatrices.begin(), boneMatrices.begin() + boneCount, boneBlock._finalBones);

      _boneUBO->set(boneBlock);
    }
  }
}

void Model::draw(std::shared_ptr<ShaderProgram> shader) {
  if (!shader) {
    Core::Logger::error("Model", "Cannot draw: shader is null");
    return;
  }

  if (_meshAssets.empty()) {
    Core::Logger::debug("Model", "Attempting to draw empty model");
    return;
  }

  if (_boneUBO) {
    _boneUBO->bind();
  }

  shader->use();

  resetMaterialState(shader);

  for (const auto &component : _meshComponents) {
    const auto &meshAsset = _meshAssets[component->getMeshAssetId()];
    const auto &texSlots = meshAsset->getTextureSlots();
    if (texSlots.empty()) {
      Core::Logger::debug("Model", "Skipping mesh '", meshAsset->getName(), "' (no textures)");
      continue;
    }

    setMaterialFlagsFromSlots(shader, texSlots);
    const auto texturesBound = bindTextures(texSlots);

    component->draw();

    if (texturesBound == 0 && !texSlots.empty()) {
      Core::Logger::warning("Model", "Mesh '", meshAsset->getName(), "' has textures but none were bound");
    }
  }
}

void Model::setAnimation(std::shared_ptr<Resources::AnimationAsset> pAnimation) {
  if (_skeletonAnimator.has_value()) {
    _skeletonAnimator.value()->playAnimation(pAnimation);
    return;
  }
  Core::Logger::warning("Model", "Model has no animator. Cannot set animation.");
}

const std::vector<std::shared_ptr<Resources::AnimationAsset>> &Model::getAnimations() const {
  if (_animationAssets.empty()) {
    Core::Logger::warning("Model", "No animations found");
  }
  return _animationAssets;
}

uint8_t Model::bindTextures(const std::vector<Resources::MeshAsset::TextureSlot> &slots) const {
  if (slots.empty()) {
    Core::Logger::warning("Model", "Cannot bind textures: slots are empty");
    return 0;
  }

  uint8_t texturesBound = 0;

  for (const auto &texSlot : slots) {
    if (texSlot.id >= _textureAssets.size()) {
      Core::Logger::warning("Model", "Invalid texture index: ", texSlot.id, " (max: ", _textureAssets.size() - 1, ")");
      continue;
    }

    const auto &textureAsset = _textureAssets[texSlot.id];
    const auto &texture = textureAsset->texture;

    if (!texture) {
      Core::Logger::warning("Model", "Null texture at index: ", texSlot.id, " (name: ", textureAsset->name, ")");
      continue;
    }

    Core::Logger::trace("Model", "Binding texture: ", textureAsset->name, " (index: ", texSlot.id,
                        ", type: ", static_cast<int>(texSlot.type), ")");

    const uint8_t textureUnit = textureUnitForType(texSlot.type);
    texture->bindTexture2D(textureUnit);
    ++texturesBound;
  }

  return texturesBound;
}
}  // namespace Render