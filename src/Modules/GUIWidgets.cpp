#include "GUIWidgets.h"

#include <imgui.h>

namespace Modules {
void createLightEditorWidget(Render::Light::LightObject& lightObject) {
  ImGui::Begin(lightObject.name.c_str());

  ImGui::SliderFloat("X", &lightObject.data._lightPosition.value.x, -10.f,
                     10.f);

  ImGui::SliderFloat("Y", &lightObject.data._lightPosition.value.y, -10.f,
                     10.f);

  ImGui::SliderFloat("Z", &lightObject.data._lightPosition.value.z, -10.f,
                     10.f);

  float lightColor[3] = {
      lightObject.data._lightColor.value.x,
      lightObject.data._lightColor.value.y,
      lightObject.data._lightColor.value.z,
  };
  ImGui::ColorEdit3("Light Color", lightColor);
  lightObject.data._lightColor = {lightColor[0], lightColor[1], lightColor[2]};

  auto ambientComponent = lightObject.data._ambientFactor.value.x;
  auto diffuseComponent = lightObject.data._diffuseFactor.value.x;
  auto specularComponent = lightObject.data._specularFactor.value.x;

  ImGui::SliderFloat("Ambient", &ambientComponent, 0.0f, 1.f);
  ImGui::SliderFloat("Diffuse", &diffuseComponent, 0.0f, 1.f);
  ImGui::SliderFloat("Specular", &specularComponent, 0.0f, 1.f);

  lightObject.data._ambientFactor = {ambientComponent, ambientComponent,
                                     ambientComponent};
  lightObject.data._diffuseFactor = {diffuseComponent, diffuseComponent,
                                     diffuseComponent};
  lightObject.data._specularFactor = {specularComponent, specularComponent,
                                      specularComponent};

  ImGui::End();
}

void createFpsWidget(float time4Frame) {
  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.3f);
  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
      ImGuiWindowFlags_NoNav;

  ImGui::Begin("FPS Overlay", nullptr, flags);

  auto fps = 1.0f / time4Frame;
  ImGui::Text("FPS: %.1f", fps);
  ImGui::Text("Frame: %.3f ms", 1000.0f / fps);

  ImGui::End();
}

}  // namespace Modules