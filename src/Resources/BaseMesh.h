#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string>

#include "../Render/RendererFactory.h"

namespace Render {
class ShaderProgram;
}

class BaseMesh {
 public:
  // Constructor
  BaseMesh(const aiMesh* pMesh, const aiScene* pScene);
  // Copy constructor
  BaseMesh(const BaseMesh&) = default;
  // Move constructor
  BaseMesh(BaseMesh&&) noexcept = default;
  // Destructor
  ~BaseMesh() noexcept = default;

  // Structure to store texture information associated with the mesh
  struct Texture {
    Texture(unsigned int id, aiTextureType type) : _id(id), _type(type) {}
    unsigned int _id;
    aiTextureType _type;
  };

  // Draw the mesh
  void drawMesh();

  // Get the list of textures associated with this mesh
  std::vector<Texture> getTexture() { return _vecTextures; }

  // Get the name of the mesh
  std::string getMeshName() { return _nameMesh; }

 private:
  // Render::Renderer _renderer;
  std::shared_ptr<Render::Renderer> _renderer;
  std::string _nameMesh;

  std::vector<Texture> _vecTextures;
};
