// clang-format off

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "Resources/ResourceManager.h"
#include "Render/ShaderProgram.h"
#include "Render/Window.h"
#include "Render/Camera.h"
#include "Render/Light.h"
#include "Render/AnimatedSprite2D.h"
#include "Modules/GUIModule.h"
#include "Modules/Time.h"
#include "Modules/Random.h"
#include "Modules/GUIWidgets.h"
#include "Modules/Logger.h"

// clang-format on

int main(int argc, char **argv) {
  (void)argc; // Unused parameter

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
  ResourceManager resourceManager(argv[0]);

  /*load SHADERS*/
  resourceManager.loadShaders();

  /*init SHADERS*/
  auto pMainShader = resourceManager.getShaderProgram("MainShader");
  auto pLightShader = resourceManager.getShaderProgram("LightShader");
  auto pDebugGridShader = resourceManager.getShaderProgram("DebugGridShader");
  auto pSprite2DShader = resourceManager.getShaderProgram("Sprite2DShader");

  /*init DEBUG GRID*/
  auto DebugGridRender = Render::RendererFactory::CreateDebugGridRenderer();

  /*CUBE*/
  auto MeshDebugCube =
      resourceManager.loadModelMesh("DebugCube", "res/models/sold.glb");

  /*LIGHT*/
  Render::Light DebugLight;

  auto MeshDebugLight = resourceManager.loadModelMesh(
      "DebugLight", "res/models/light-sphere.glb");

  Render::Light::LightObject lightObject_1{
      "light_1",
      {},
      MeshDebugLight,
  };
  lightObject_1.data._lightPosition = {0.f, 5.f, 0.f};

  Render::Light::LightObject lightObject_2{
      "light_2",
      {},
      MeshDebugLight,
  };
  lightObject_2.data._lightPosition = {0.f, 3.f, 0.f};

  DebugLight.addLight(lightObject_1);
  DebugLight.addLight(lightObject_2);

  /*2D ANIMATED SPRITE SETUP*/
  // Load texture atlas
  std::string atlasName = "AttackAtlas";
  std::string atlasPath = "res/textures/attack.png";
  unsigned int subTexWidth = 126;   // Width of each frame in atlas
  unsigned int subTexHeight = 39;   // Height of each frame in atlas
  
  std::vector<std::string> frameNames = {
      "attack_0", "attack_1", "attack_2", "attack_3",
      "attack_4", "attack_5", "attack_6", "attack_7"
  };
  
  auto pSpriteAtlas = resourceManager.loadTextureAtlas2D(
      atlasName, atlasPath, frameNames, subTexWidth, subTexHeight);
  
  // Create animated sprite (scoped outside if for render loop access)
  std::unique_ptr<Render::AnimatedSprite2D> animatedSprite;
  
  if (pSpriteAtlas && pSprite2DShader) {
    // Create animated sprite
    glm::vec3 spritePosition(-3.0f, 0.0f, 0.0f);  // Position in 3D space
    glm::vec2 spriteSize(2.0f, 0.62f);  // World-space size (maintains aspect ratio)
    
    animatedSprite = std::make_unique<Render::AnimatedSprite2D>(
        pSpriteAtlas, pSprite2DShader, spritePosition, spriteSize, 0.0f);
    
    // Set animation parameters (frame names and durations)
    std::vector<std::pair<std::string, std::chrono::nanoseconds>> frameDurations;
    const auto frameDuration = std::chrono::milliseconds(100);  // 100ms per frame
    for (const auto& frameName : frameNames) {
      frameDurations.emplace_back(frameName, frameDuration);
    }
    animatedSprite->setAnimationParameters(frameDurations);
  }

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

    /*DRAWING CUBE*/
    pMainShader->use();

    auto modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 1.f, 1.f));

    auto shininess = 64.f;
    pMainShader->setFloatUniform("material.shininess", shininess);

    pMainShader->setMatrix4Uniform("modelMatrix", modelMatrix);

    MeshDebugCube->draw(pMainShader);

    /*DEBUG GRID*/
    pDebugGridShader->use();
    pDebugGridShader->setVec2Uniform("uViewportSize",
                                     {windowWidth, windowHeight});
    DebugGridRender->drawArrays();

    /*2D ANIMATED SPRITE*/
    if (animatedSprite) {
      // Update sprite animation using MainTimer deltaTime (in seconds)
      animatedSprite->update(deltaTime);
      
      // Render sprite
      animatedSprite->render();
    }

    /*UI*/
    Modules::GUIModule::GUIbegin();

    Modules::createFpsWidget(deltaTime);

    Modules::createLightEditorWidget(lightObject_1);
    Modules::createLightEditorWidget(lightObject_2);

    Modules::GUIModule::GUIend();

    /*update window*/
    MainWindow.update();
  }
  return 0;
}
