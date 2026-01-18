#include "ParseUtils.h"

#include "../Modules/Logger.h"
#include <cstdlib>

namespace Resources {

namespace {
/**
 * @brief Extract texture index from Assimp texture path
 *
 * Embedded textures have paths like "*0", "*1", etc. where the number after
 * '*' is the index into scene->mTextures array. File-based textures have
 * actual file paths.
 *
 * @param pathStr The texture path string from Assimp
 * @return The texture index (0 for file-based textures, parsed index for
 * embedded)
 */
[[nodiscard]] unsigned int extractTextureIndex(const char *pathStr) noexcept {
  if (!pathStr) {
    return 0;
  }

  // Embedded texture - extract index from "*N" format
  if (pathStr[0] == '*') {
    // Use strtoul for safer parsing (handles multi-digit indices)
    char *endPtr = nullptr;
    const unsigned long index = std::strtoul(pathStr + 1, &endPtr, 10);

    // Validate that we actually parsed a number
    if (endPtr != pathStr + 1 && *endPtr == '\0') {
      return static_cast<unsigned int>(index);
    }
  }

  // File-based texture - return 0 as default
  // In a full implementation, we'd match by filename
  return 0;
}

[[nodiscard]] bool validateMesh(const aiMesh *mesh) noexcept {
  if (!mesh) {
    return false;
  }

  if (mesh->mNumVertices == 0) {
    Core::Logger::error("ParseUtils", "Mesh has no vertices");
    return false;
  }

  if (!mesh->HasPositions()) {
    Core::Logger::error("ParseUtils", "Mesh has no positions");
    return false;
  }

  return true;
}

void parseGeometry(const aiMesh *mesh, MeshAsset &meshAsset) {
  const unsigned int vertexCount = mesh->mNumVertices;

  meshAsset.reserveVertices(vertexCount);

  for (unsigned int i = 0; i < vertexCount; ++i) {
    const aiVector3D &vertex = mesh->mVertices[i];
    meshAsset.addVertex(glm::vec3(vertex.x, vertex.y, vertex.z));

    if (mesh->HasNormals()) {
      const aiVector3D &normal = mesh->mNormals[i];
      meshAsset.addNormal(glm::vec3(normal.x, normal.y, normal.z));
    }

    if (mesh->HasTextureCoords(0) && mesh->mTextureCoords[0]) {
      const aiVector3D &texCoord = mesh->mTextureCoords[0][i];
      meshAsset.addTexCoord(glm::vec2(texCoord.x, texCoord.y));
    }
  }
}

void parseTextures(const aiMaterial *material, MeshAsset &meshAsset) {
  if (!material) {
    return;
  }

  // Iterate through all possible texture types
  for (int type = aiTextureType_NONE; type <= AI_TEXTURE_TYPE_MAX; ++type) {
    const aiTextureType textureType = static_cast<aiTextureType>(type);
    const unsigned int textureCount =
        material->GetTextureCount(textureType);

    if (textureCount == 0) {
      continue;
    }

    aiString path;
    material->GetTexture(textureType, 0, &path);

    const unsigned int textureIndex = extractTextureIndex(path.C_Str());
    meshAsset.addTexture(MeshAsset::Texture(textureIndex, textureType));
  }
}

void parseIndices(const aiMesh *mesh, MeshAsset &meshAsset) {
  std::vector<unsigned int> indices;
  for (int i = 0; i < mesh->mNumFaces; i++) {
    auto face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  meshAsset.setIndices(std::move(indices));
}

void parseBones(const aiMesh *mesh, MeshAsset &meshAsset) {
  if (!mesh->HasBones() || mesh->mNumBones == 0) {
    return;
  }

  const unsigned int vertexCount = mesh->mNumVertices;
  meshAsset.initializeBoneData(vertexCount);

  auto &boneData = meshAsset.getBoneDataMutable();
  if (!boneData.has_value()) {
    Core::Logger::error("ParseUtils", "Failed to initialize bone data");
    return;
  }

  // Process each bone
  for (unsigned int boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx) {
    const aiBone *bone = mesh->mBones[boneIdx];
    if (!bone) {
      Core::Logger::warning("ParseUtils", "Bone ", boneIdx, " is null");
      continue;
    }

    // Process each weight for this bone
    for (unsigned int weightIdx = 0; weightIdx < bone->mNumWeights;
         ++weightIdx) {
      const aiVertexWeight &weight = bone->mWeights[weightIdx];
      const unsigned int vertexId = weight.mVertexId;

      // Validate vertex ID
      if (vertexId >= vertexCount) {
        Core::Logger::warning("ParseUtils",
                              "Bone weight references invalid vertex: ",
                              vertexId);
        continue;
      }

      auto &vertexBoneData = (*boneData)[vertexId];

      // Find an empty slot for this bone weight
      bool slotFound = false;
      for (size_t slot = 0; slot < MeshAsset::kMaxBonePerVertex; ++slot) {
        if (vertexBoneData.boneWeights[slot] == 0.0f) {
          vertexBoneData.boneIds[slot] = boneIdx;
          vertexBoneData.boneWeights[slot] = weight.mWeight;
          slotFound = true;
          break;
        }
      }

      if (!slotFound) {
        Core::Logger::warning(
            "ParseUtils",
            "Vertex ", vertexId,
            " has more than ", MeshAsset::kMaxBonePerVertex,
            " bone influences. Some weights will be ignored.");
      }
    }
  }
}

} // anonymous namespace

std::shared_ptr<MeshAsset> parseMesh(const aiMesh *mesh,
                                     const aiMaterial *material) {
  if (!mesh) {
    Core::Logger::error("ParseUtils", "Mesh is nullptr");
    return nullptr;
  }

  if (!material) {
    Core::Logger::error("ParseUtils", "Material is nullptr");
    return nullptr;
  }

  if (!validateMesh(mesh)) {
    return nullptr;
  }

  auto meshAsset = std::make_shared<MeshAsset>();

  meshAsset->setName(mesh->mName.C_Str());

  parseGeometry(mesh, *meshAsset);

  parseIndices(mesh, *meshAsset);

  parseTextures(material, *meshAsset);

  parseBones(mesh, *meshAsset);

  if (!meshAsset->isValid()) {
    Core::Logger::error("ParseUtils", "Parsed mesh has no vertices");
    return nullptr;
  }

  return meshAsset;
}

} // namespace Resources
