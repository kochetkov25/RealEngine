#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string>

#include "Render/RendererFactory.h"

namespace Render {
class ShaderProgram;
}

class BaseMesh {
 public:
  BaseMesh(const aiMesh *pMesh, const aiScene *pScene);
  BaseMesh(const BaseMesh &) = default;
  BaseMesh(BaseMesh &&) noexcept = default;
  ~BaseMesh() noexcept = default;

  struct Texture {
    Texture(unsigned int id, aiTextureType type) : _id(id), _type(type) {}
    unsigned int _id;
    aiTextureType _type;
  };

  void drawMesh() const;

  [[nodiscard]] const std::vector<Texture> &getTextures() const noexcept { return _vecTextures; }

  [[nodiscard]] std::vector<Texture> getTexture() const { return _vecTextures; }

  std::string getMeshName() { return _nameMesh; }

 private:
  std::shared_ptr<Render::Renderer> _renderer;
  std::string _nameMesh;

  std::vector<Texture> _vecTextures;
};
