#pragma once

#include <vector>

#include "Render/Light.h"

namespace Modules {
void createLightEditorWidget(Render::Light::LightObject& lightObject);

void createFpsWidget(float time4Frame);

class ModelEditorWidget {
 public:
  ModelEditorWidget() = default;
  ~ModelEditorWidget() = default;

  void addModelMeta(std::shared_ptr<Render::ModelMeta> modelMeta);

  void drawHierarchy();

  void drawEditor();

 private:
  std::vector<std::shared_ptr<Render::ModelMeta>> _modelMetas;

  std::shared_ptr<Render::ModelMeta> _selectedModelMeta = nullptr;
};
}  // namespace Modules