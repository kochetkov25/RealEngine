#include "BaseMesh.h"

#include <iostream>

#include "Render/RendererFactory.h"

// Constructor
BaseMesh::BaseMesh(const aiMesh* pMesh, const aiScene* pScene) {
  // Validate input parameters
  if (!pMesh) {
    std::cerr << "Mesh is nullptr! (source: " << __FUNCTION__ << ")"
              << std::endl;
    return;
  }
  if (!pScene) {
    std::cerr << "Scene is nullptr! (source: " << __FUNCTION__ << ")"
              << std::endl;
    return;
  }
  if (!pMesh->HasNormals()) {
    std::cerr << "No normals! (source: " << __FUNCTION__ << ")" << std::endl;
    return;
  }
  if (!pMesh->HasPositions()) {
    std::cerr << "No positions! (source: " << __FUNCTION__ << ")" << std::endl;
    return;
  }

  // Store mesh name
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
    std::cout << "NAME: " << _nameMesh << " CNT: " << cnt << std::endl;
    if (cnt > 0) {
      aiString path;
      pMaterial->GetTexture(static_cast<aiTextureType>(i), 0, &path);
      unsigned int index = path.C_Str()[1] - '0';

      _vecTextures.push_back(Texture(index, static_cast<aiTextureType>(i)));
    }
  }
  std::cout << std::endl;

  _renderer->upload();
}

// Draw the mesh using indexed rendering
void BaseMesh::drawMesh() { _renderer->drawElements(); }
