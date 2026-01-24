#include "ModelMesh.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <unordered_map>

#include "../Modules/Logger.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"


ModelMesh::ModelMesh(const aiScene *pScene, VecTexGL vecTexGL) {
  _vecTexGL = vecTexGL;
  processNode(pScene->mRootNode, pScene);
}

void ModelMesh::processNode(const aiNode *pNode, const aiScene *pScene) {
  for (unsigned int i = 0; i < pNode->mNumMeshes; i++) {
    auto mesh = pScene->mMeshes[pNode->mMeshes[i]];
    _vecMeshes.push_back(std::make_shared<BaseMesh>(mesh, pScene));
  }

  for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
    processNode(pNode->mChildren[i], pScene);
  }
}

void ModelMesh::draw(std::shared_ptr<Render::ShaderProgram> shader, bool skipEmptyMeshes) const {
  if (!shader) {
    Core::Logger::error("ModelMesh", "Cannot draw: shader is null");
    return;
  }

  if (_vecMeshes.empty()) {
    Core::Logger::debug("ModelMesh", "Attempting to draw empty model");
    return;
  }

  shader->use();

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

  for (const auto &mesh : _vecMeshes) {
    if (!mesh) {
      Core::Logger::warning("ModelMesh", "Null mesh encountered, skipping");
      continue;
    }

    const auto &meshTextures = mesh->getTextures();

    if (skipEmptyMeshes && meshTextures.empty()) {
      Core::Logger::debug("ModelMesh", "Skipping mesh '", mesh->getMeshName(), "' (no textures)");
      continue;
    }

    const uint8_t texturesBound = bindMeshTextures(meshTextures, shader);

    mesh->drawMesh();

    if (texturesBound == 0 && !meshTextures.empty()) {
      Core::Logger::warning("ModelMesh", "Mesh '", mesh->getMeshName(), "' has textures but none were bound");
    }
  }
}

uint8_t ModelMesh::bindMeshTextures(const std::vector<BaseMesh::Texture> &meshTextures,
                                    std::shared_ptr<Render::ShaderProgram> shader) const {
  if (meshTextures.empty() || !shader) {
    return 0;
  }

  uint8_t texturesBound = 0;

  // Track which texture types we've bound to avoid duplicates
  // In a typical PBR setup, we want: diffuse, specular, normal, etc.
  // Map texture type to texture unit for consistent binding
  std::unordered_map<aiTextureType, uint8_t> typeToUnitMap;
  uint8_t nextAvailableUnit = 0;

  // Standard texture type to unit mapping (PBR-compatible)
  constexpr uint8_t kMaxTextureUnits = 16;  // Conservative limit

  for (const auto &meshTex : meshTextures) {
    if (meshTex._id >= _vecTexGL.size()) {
      Core::Logger::warning("ModelMesh", "Invalid texture index: ", meshTex._id, " (max: ", _vecTexGL.size() - 1, ")");
      continue;
    }

    const auto &texturePair = _vecTexGL[meshTex._id];
    const auto &texture = texturePair.second;

    if (!texture) {
      Core::Logger::warning("ModelMesh", "Null texture at index: ", meshTex._id, " (name: ", texturePair.first, ")");
      continue;
    }

    Core::Logger::trace("ModelMesh", "Binding texture: ", texturePair.first, " (index: ", meshTex._id,
                        ", type: ", static_cast<int>(meshTex._type), ")");

    // Determine texture unit based on type (reuse same unit for same type)
    uint8_t textureUnit = 0;
    const auto typeIt = typeToUnitMap.find(meshTex._type);

    if (typeIt != typeToUnitMap.end()) {
      // Reuse existing unit for this texture type
      textureUnit = typeIt->second;
    } else {
      // Assign new unit for this texture type
      if (nextAvailableUnit >= kMaxTextureUnits) {
        Core::Logger::warning("ModelMesh", "Maximum texture units (", kMaxTextureUnits, ") reached, skipping texture");
        continue;
      }
      textureUnit = nextAvailableUnit++;
      typeToUnitMap[meshTex._type] = textureUnit;
    }

    texture->bindTexture2D(textureUnit);

    std::string uniformName = "material.texture";
    std::string flagName = "";

    switch (meshTex._type) {
      case aiTextureType_DIFFUSE:
      case aiTextureType_BASE_COLOR:
        uniformName = "material.diffuse";
        flagName = "hasMaterialDiffuse";
        // Set legacy uniform for backward compatibility
        shader->setTexUniform("material.texture", static_cast<GLint>(textureUnit));
        shader->setBoolUniform("hasMaterialTexture", true);
        break;
      case aiTextureType_SPECULAR:
        uniformName = "material.specular";
        flagName = "hasMaterialSpecular";
        // Set legacy uniform
        shader->setTexUniform("material.specularMap", static_cast<GLint>(textureUnit));
        shader->setBoolUniform("hasMaterialSpecularMap", true);
        break;
      case aiTextureType_NORMALS:
      case aiTextureType_NORMAL_CAMERA:
        uniformName = "material.normal";
        flagName = "hasMaterialNormal";
        shader->setBoolUniform("useNormalMapping", true);
        break;
      case aiTextureType_HEIGHT:
      case aiTextureType_DISPLACEMENT:
        uniformName = "material.height";
        flagName = "hasMaterialHeight";
        break;
      case aiTextureType_AMBIENT:
      case aiTextureType_AMBIENT_OCCLUSION:
        uniformName = "material.ambient";
        flagName = "hasMaterialAmbient";
        break;
      case aiTextureType_EMISSIVE:
        uniformName = "material.emissive";
        flagName = "hasMaterialEmissive";
        // Set legacy uniform
        shader->setTexUniform("material.emissionMap", static_cast<GLint>(textureUnit));
        shader->setBoolUniform("hasMaterialEmissionMap", true);
        break;
      case aiTextureType_METALNESS:
        uniformName = "material.metallic";
        flagName = "hasMaterialMetallic";
        break;
      case aiTextureType_DIFFUSE_ROUGHNESS:
        uniformName = "material.roughness";
        flagName = "hasMaterialRoughness";
        break;
      default:
        // Fallback to generic texture uniform
        uniformName = "material.texture";
        flagName = "hasMaterialTexture";
        break;
    }

    shader->setTexUniform(uniformName, static_cast<GLint>(textureUnit));

    if (!flagName.empty()) {
      shader->setBoolUniform(flagName, true);
    }

    ++texturesBound;
  }

  return texturesBound;
}