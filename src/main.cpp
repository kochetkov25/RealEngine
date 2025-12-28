// clang-format off

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "Resources/ResourceManager.h"
#include "Render/ShaderProgram.h"
#include "Render/Window.h"
#include "Render/Camera.h"
#include "Render/Light.h"
#include "Modules/GUIModule.h"
#include "Modules/Time.h"
#include "Modules/Random.h"
#include "Modules/GUIWidgets.h"

// clang-format on

int main(int argc, char** argv) {
  (void)argc;  // Unused parameter
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

  /*load TEXTURES*/
  // resourceManager.loadTexture2D("BOX_TEX",  "res/textures/body_lstr.png");
  // resourceManager.loadTexture2D("SpecularMap",
  // "res/textures/BOX_specularMap.png");
  // resourceManager.loadTexture2D("EmissionMap",
  // "res/textures/BOX_emissionMAP.png"); auto currTex =
  // resourceManager.getTexture2D("BOX_TEX"); auto specularTex  =
  // resourceManager.getTexture2D("SpecularMap"); auto emissionMap =
  // resourceManager.getTexture2D("EmissionMap");

  /*init SHADERS*/
  auto pMainShader = resourceManager.getShaderProgram("MainShader");
  auto pLightShader = resourceManager.getShaderProgram("LightShader");
  auto pDebugGridShader = resourceManager.getShaderProgram("DebugGridShader");

  /*init DEBUG GRID*/
  auto DebugGridRender = Render::RendererFactory::CreateDebugGridRenderer();

  /*CUBE*/
  auto MeshDebugCube =
      resourceManager.loadModelMesh("DebugCube", "res/models/CubeNormals.glb");

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
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f, 2.f, 2.f));

    ///*TEXTURE*/
    // currTex->bindTexture2D(0);
    // pShaderProg->setTexUniform("material.texture", 0);

    ///*SPECULAT MAP*/
    // specularTex->bindTexture2D(1);
    // pShaderProg->setTexUniform("material.specularMap", 1);

    ///*EMISSION MAP*/
    // emissionMap->bindTexture2D(2);
    // pShaderProg->setTexUniform("material.emissionMap", 2);

    auto shininess = 32.f;
    pMainShader->setFloatUniform("material.shininess", shininess);

    pMainShader->setMatrix4Uniform("modelMatrix", modelMatrix);

    MeshDebugCube->draw(pMainShader);

    /*DEBUG GRID*/
    pDebugGridShader->use();
    pDebugGridShader->setVec2Uniform("uViewportSize",
                                     {windowWidth, windowHeight});
    DebugGridRender->drawArrays();

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
