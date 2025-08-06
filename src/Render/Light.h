#pragma once

#include <unordered_map>

#include "Render/ShaderUtils/UniformLight.h"
#include "Render/UniformBuffer.h"
#include "Resources/ModelMesh.h"

namespace Render {

class Light {
 public:
  struct LightObject {
    std::string name;

    ShaderUtils::LightData data;
    std::shared_ptr<ModelMesh> mesh;
  };

  Light();

  void addLight(const LightObject& light);

  void rewriteLightData(const std::string& name,
                        const ShaderUtils::LightData& data);

  void moveLight(const std::string& name, const glm::vec3& pos);

  void draw(std::shared_ptr<Render::ShaderProgram> shader);

 private:
  std::unordered_map<std::string, uint16_t> _name2uid;
  std::vector<std::shared_ptr<ModelMesh>> _lightMeshes;

  ShaderUtils::LightBlock _lightBlock;
  UniformBuffer<ShaderUtils::LightBlock> _lightUBO;
};

}  // namespace Render