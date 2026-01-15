#include "BaseMesh.h"

#include "../Modules/Logger.h"
#include "Render/RendererFactory.h"

BaseMesh::BaseMesh(const aiMesh *pMesh, const aiScene *pScene) {
  if (!pMesh) {
    Core::Logger::error("BaseMesh", "Mesh is nullptr!");
    return;
  }
  if (!pScene) {
    Core::Logger::error("BaseMesh", "Scene is nullptr!");
    return;
  }
  if (!pMesh->HasNormals()) {
    Core::Logger::error("BaseMesh", "Mesh has no normals!");
    return;
  }
  if (!pMesh->HasPositions()) {
    Core::Logger::error("BaseMesh", "Mesh has no positions!");
    return;
  }

  _nameMesh = pMesh->mName.C_Str();

  // Create renderer for this mesh
  _renderer = Render::RendererFactory::CreateDefault3DModelRenderer();

  // Process vertices: position, UV coordinates, and normals
  for (int i = 0; i < pMesh->mNumVertices; i++) {
    // Add vertex position
    _renderer->vertex3(pMesh->mVertices[i].x, pMesh->mVertices[i].y,
                       pMesh->mVertices[i].z);
    // Add texture coordinates
    _renderer->vertexUV(pMesh->mTextureCoords[0][i].x,
                        pMesh->mTextureCoords[0][i].y);
    // Add normal
    _renderer->vertex3(pMesh->mNormals[i].x, pMesh->mNormals[i].y,
                       pMesh->mNormals[i].z);
  }

  // Extract indices for indexed rendering
  std::vector<GLuint> indices;
  for (int i = 0; i < pMesh->mNumFaces; i++) {
    auto face = pMesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  _renderer->setIndices(indices);

  // Process materials and textures associated with this mesh
  auto pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
  for (int i = aiTextureType_NONE; i <= AI_TEXTURE_TYPE_MAX; i++) {
    auto cnt = pMaterial->GetTextureCount(static_cast<aiTextureType>(i));
    if (cnt > 0) {
      aiString path;
      pMaterial->GetTexture(static_cast<aiTextureType>(i), 0, &path);

      // Extract texture index from path
      // Embedded textures have paths like "*0", "*1", etc. where the number
      // after '*' is the index into scene->mTextures array
      unsigned int index = 0;
      const char *pathStr = path.C_Str();

      if (pathStr && pathStr[0] == '*') {
        // Embedded texture - extract index from "*N" format
        // Handle both single digit ("*0") and multi-digit ("*10") indices
        if (pathStr[1] >= '0' && pathStr[1] <= '9') {
          // Try to parse as integer (handles multi-digit)
          index = static_cast<unsigned int>(std::atoi(pathStr + 1));
        }
      } else {
        // File-based texture - for now use index 0
        // In a full implementation, we'd match by filename
        index = 0;
      }

      _vecTextures.push_back(Texture(index, static_cast<aiTextureType>(i)));
    }
  }

  _renderer->upload();
}

void BaseMesh::drawMesh() const {
  if (!_renderer) {
    Core::Logger::error("BaseMesh", "Renderer is null!");
    return;
  }
  _renderer->drawElements();
}
