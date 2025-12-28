#include "Render/Light.h"

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/ShaderProgram.h"

Render::Light::Light() : _lightBlock(), _lightUBO(1) {}

void Render::Light::addLight(const LightObject& light) {
  if (_name2uid.count(light.name)) {
    assert(false && "Light with this name already exist!");
  }

  if (_lightBlock._count + 1 >= ShaderUtils::kMaxLightsCount) {
    assert(false && "Light array overflow!");
  }

  _lightMeshes.push_back(light.mesh);
  _name2uid[light.name] = _lightBlock._count;

  _lightBlock._data[_lightBlock._count] = light.data;
  _lightBlock._count++;
}

void Render::Light::rewriteLightData(const std::string& name,
                                     const ShaderUtils::LightData& data) {
  if (!_name2uid.count(name)) {
    assert(false && "Light doesn't exist!");
  }

  _lightBlock._data[_name2uid.at(name)] = data;
}

void Render::Light::moveLight(const std::string& name, const glm::vec3& pos) {
  if (!_name2uid.count(name)) {
    assert(false && "Light doesn't exist!");
  }

  _lightBlock._data[_name2uid.at(name)]._lightPosition = pos;
}

void Render::Light::draw(std::shared_ptr<Render::ShaderProgram> shader) {
  // Should use lights before other objects. May need refactoring.
  _lightUBO.set(_lightBlock);

  shader->use();

  for (auto [name, uid] : _name2uid) {
    auto modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix,
                                 _lightBlock._data[uid]._lightPosition.value);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));

    shader->setMatrix4Uniform("modelMatrix", modelMatrix);
    shader->setVec3Uniform("lightColor", _lightBlock._data[uid]._lightColor);

    _lightMeshes.at(uid)->draw(shader);
  }
}
