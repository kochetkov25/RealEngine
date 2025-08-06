#pragma once

#include <memory>
#include <vector>

#include "BaseMesh.h"

struct aiScene;

namespace Render {
class Texture2D;
class ShaderProgram;
}  // namespace Render

class ModelMesh {
 public:
  typedef std::vector<
      std::pair<std::string, std::shared_ptr<Render::Texture2D>>>
      VecTexGL;
  ModelMesh(const aiScene* pScene, VecTexGL vecTexGL);

  void processNode(const aiNode* pNode, const aiScene* pScene);

  void draw(std::shared_ptr<Render::ShaderProgram> shader);

 private:
  std::vector<std::shared_ptr<BaseMesh>> _vecMeshes;

  VecTexGL _vecTexGL;
};