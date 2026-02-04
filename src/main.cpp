// clang-format off

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Resources/FileManager.h"
#include "Resources/ResourceManager.h"
#include "Render/ShaderProgram.h"
#include "Render/Window.h"
#include "Render/Camera.h"
#include "Render/Light.h"
#include "Render/AnimatedSprite2D.h"
#include "Render/RendererFactory.h"
#include "Modules/GUIModule.h"
#include "Modules/Time.h"
#include "Modules/Random.h"
#include "Modules/GUIWidgets.h"
#include "Modules/Logger.h"
#include "Resources/ShaderManager.h"
#include "Render/Model.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

// clang-format on

int main(int argc, char **argv) {
  (void)argc;  // Unused parameter

  Resources::FileManager::instance().initialize(argv[0]);

  Core::Logger::setLogLevel(Core::Logger::LogLevel::Debug);

  /*init RANDOM*/
  Core::Random::Init();

  /*init TIMER*/
  Core::Time MainTimer;

  /*init MAIN WINDOW*/
  Render::Window MainWindow;
  if (!MainWindow.init()) {
    assert(false && "Failed to init MainWindow! Critical!");
  }

  MainWindow.setRenderMode(Render::Window::RenderMode::Fill);

  /*init UI*/
  Modules::GUIModule::onWindowCreate(MainWindow.getWindow());

  /*init CAMERA*/
  Render::Camera MainCamera;

  /*CAMERA properties*/
  auto windowHeight = static_cast<float>(MainWindow.getHeight());
  auto windowWidth = static_cast<float>(MainWindow.getWidth());
  MainCamera.setWindowSize(windowHeight, windowWidth);

  glm::vec3 cameraPosition(0.0f, 2.0f, 7.0f);
  MainCamera.setPosition(cameraPosition);

  MainCamera.setPlane(0.1f, 500.f);
  MainCamera.setVelocity(10.f);
  MainCamera.setProjectionMode(Render::Camera::ProjectionMode::PERSPECTIVE);

  /*init RESOURCE MANAGER*/
  ResourceManager resourceManager;

  /*get SHADERS*/
  auto pMeshShader = resourceManager.getShaderProgram(Resources::ShaderType::Mesh);
  auto pSkinnedMeshShader =
      resourceManager.getShaderProgram(Resources::ShaderType::Mesh, Resources::ShaderFeatures::Skinned);
  auto pLightShader = resourceManager.getShaderProgram(Resources::ShaderType::Light);
  auto pDebugGridShader = resourceManager.getShaderProgram(Resources::ShaderType::DebugGrid);
  auto pSprite2DShader = resourceManager.getShaderProgram(Resources::ShaderType::Sprite2D);

  /*init DEBUG GRID*/
  auto DebugGridRender = Render::RendererFactory::CreateDebugGridRenderer();

  /*UI MODEL EDITOR*/
  Modules::ModelEditorWidget modelEditorWidget;

  /*3D MODELS*/
  auto model_anim_1 = resourceManager.loadModel("Medicccc", "res/models/nono.glb");
  model_anim_1->setAnimation(model_anim_1->getAnimations().front());
  model_anim_1->getModelMeta()->_position = {-3.f, 0.f, -2.f};
  model_anim_1->getModelMeta()->_scale = 2;
  modelEditorWidget.addModelMeta(model_anim_1->getModelMeta());

  auto model_anim_2 = resourceManager.loadModel("Neiro", "res/models/neiro.glb");
  model_anim_2->setAnimation(model_anim_2->getAnimations().front());
  model_anim_2->getModelMeta()->_position = {2.4f, 1.6f, 3.f};
  model_anim_2->getModelMeta()->_scale = 2;
  modelEditorWidget.addModelMeta(model_anim_2->getModelMeta());

  auto model_stat_1 = resourceManager.loadModel("Cube", "res/models/CubeNormals.glb");
  model_stat_1->getModelMeta()->_position = {-0, -5, 0};
  model_stat_1->getModelMeta()->_scale = 5;
  modelEditorWidget.addModelMeta(model_stat_1->getModelMeta());

  /*LIGHT*/
  Render::Light DebugLight;

  auto ModelDebugLight = resourceManager.loadModel("DebugLight", "res/models/light-sphere.glb");

  Render::Light::LightObject lightObject_1{
      "light_1",
      {},
      ModelDebugLight,
  };
  lightObject_1.data._position.value = glm::vec4(2.f, 7.f, 0.f, 0.f);

  Render::Light::LightObject lightObject_2{
      "light_2",
      {},
      ModelDebugLight,
  };
  lightObject_2.data._position.value = glm::vec4(-2.f, 5.f, 0.f, 0.f);

  DebugLight.addLight(lightObject_1);
  DebugLight.addLight(lightObject_2);

  /*2D ANIMATED SPRITE SETUP*/
  std::string atlasName = "AttackAtlas";
  std::string atlasPath = "res/textures/loading_3.png";

  unsigned int subTexWidth = 256;
  unsigned int subTexHeight = 256;

  unsigned int framesCount = 20;

  std::vector<std::string> frameNames;
  for (unsigned int spriteInd = 0; spriteInd < framesCount; ++spriteInd) {
    frameNames.push_back("frame_" + std::to_string(spriteInd));
  }

  auto pSpriteAtlas = resourceManager.loadTextureAtlas2D(atlasName, atlasPath, frameNames, subTexWidth, subTexHeight);

  std::unique_ptr<Render::AnimatedSprite2D> animatedSprite;

  glm::vec3 spritePosition(-3.0f, 0.0f, 0.0f);  // Position in 3D space
  glm::vec2 spriteSize(1.0f, 1.0f);             // World-space size (maintains aspect ratio)

  animatedSprite =
      std::make_unique<Render::AnimatedSprite2D>(pSpriteAtlas, pSprite2DShader, spritePosition, spriteSize, 0.0f);

  std::vector<std::pair<std::string, std::chrono::nanoseconds>> frameDurations;
  const auto frameDuration = std::chrono::milliseconds(50);
  for (const auto &frameName : frameNames) {
    frameDurations.emplace_back(frameName, frameDuration);
  }
  animatedSprite->setAnimationParameters(frameDurations);

  /*GL CONTEXT*/
  /*enable Depth Buffer*/
  glEnable(GL_DEPTH_TEST);
  /*enable blending*/
  glEnable(GL_BLEND);
  /*set blending settings*/
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /*enable MSAA*/
  glEnable(GL_MULTISAMPLE);

  while (!MainWindow.windowShouldClose()) {
    /*TIMER*/
    auto deltaTime = MainTimer.stop() / 1000.f;
    MainTimer.start();

    /*CAMERA*/
    MainCamera.moveCamera(deltaTime);
    MainCamera.update();

    /*DRAWING LIGHT*/
    DebugLight.rewriteLightData("light_1", lightObject_1.data);
    DebugLight.rewriteLightData("light_2", lightObject_2.data);

    DebugLight.draw(pLightShader);

    /*SKINNED MODEL 1*/
    pSkinnedMeshShader->use();

    pSkinnedMeshShader->setFloatUniform("material.shininess", model_anim_1->getModelMeta()->_shininess);
    pSkinnedMeshShader->setMatrix4Uniform("modelMatrix", model_anim_1->getModelMeta()->getModelMatrix());

    model_anim_1->update(deltaTime);
    model_anim_1->draw(pSkinnedMeshShader);

    /*SKINNED MODEL 2*/
    pSkinnedMeshShader->setFloatUniform("material.shininess", model_anim_2->getModelMeta()->_shininess);
    pSkinnedMeshShader->setMatrix4Uniform("modelMatrix", model_anim_2->getModelMeta()->getModelMatrix());

    model_anim_2->update(deltaTime);
    model_anim_2->draw(pSkinnedMeshShader);

    /*STATIC MODEL 1*/
    pMeshShader->use();

    pMeshShader->setFloatUniform("material.shininess", model_stat_1->getModelMeta()->_shininess);
    pMeshShader->setMatrix4Uniform("modelMatrix", model_stat_1->getModelMeta()->getModelMatrix());

    model_stat_1->draw(pMeshShader);

    /*DEBUG GRID*/
    pDebugGridShader->use();
    pDebugGridShader->setVec2Uniform("uViewportSize", {windowWidth, windowHeight});
    DebugGridRender->drawArrays();

    /*2D ANIMATED SPRITE*/
    animatedSprite->update(deltaTime);
    animatedSprite->render();

    /*UI*/
    Modules::GUIModule::GUIbegin();

    Modules::createFpsWidget(deltaTime);

    Modules::createLightEditorWidget(lightObject_1);
    Modules::createLightEditorWidget(lightObject_2);

    modelEditorWidget.drawHierarchy();
    modelEditorWidget.drawEditor();

    Modules::GUIModule::GUIend();

    /*update window*/
    MainWindow.update();
  }
  return 0;
}
