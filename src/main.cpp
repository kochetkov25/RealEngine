// clang-format off
#include "Resources/ResourceManager.h"

#include "Render/ShaderProgram.h"
#include "Render/Texture2D.h"
#include "Render/Sprite.h"
#include "Render/AnimatedSprite.h"
#include "Render/Window.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/RendererFactory.h"
#include "Render/Camera.h"
#include "Render/UniformBuffer.h"

#include "Modules/GUIModule.h"
#include "Modules/Time.h"
#include "Modules/Random.h"

#include "Input/Input.h"

#include <imgui.h>


#include <assimp/Importer.hpp>

#include "Resources/ModelMesh.h"
// clang-format on

namespace Render {
extern void createRawCube(std::shared_ptr<Renderer> render);
extern void createTexCube(std::shared_ptr<Renderer> render);
extern void createTexCubeLight(std::shared_ptr<Renderer> render);
extern void createTexCubeLight_Indices(std::shared_ptr<Renderer> render);
extern void createXYplane(std::shared_ptr<Renderer> render);
}  // namespace Render

int main(int argc, char** argv) {
  /*RANDOM*/
  Core::Random::Init();

  /*creatin MAIN WINDOW*/
  Render::Window MainWindow;
  if (!MainWindow.init()) return -1;

  /*creating UI*/
  Modules::GUIModule::onWindowCreate(MainWindow.getWindow());

  /*creatin CAMERA*/
  Render::Camera MainCamera;

  /*CAMERA properties*/
  MainCamera.setWindowSize(static_cast<float>(MainWindow.getHeight()),
                           static_cast<float>(MainWindow.getWidth()));
  glm::vec3 cameraPosition(0.0f, 2.0f, 7.0f);
  MainCamera.setPosition(cameraPosition);
  MainCamera.setPlane(0.1f, 500.f);
  MainCamera.setVelocity(10.f);
  MainCamera.setProjectionMode(Render::Camera::ProjectionMode::PERSPECTIVE);

  /*creating RESOURCE MANAGER*/
  ResourceManager resourceManager(argv[0]);

  /*load SHADERS*/
  resourceManager.loadShaders();
  std::string shaderName = "SpriteShader";

  /*load TEXTURES*/
  // resourceManager.loadTexture2D("BOX_TEX",  "res/textures/body_lstr.png");
  // resourceManager.loadTexture2D("SpecularMap",
  // "res/textures/BOX_specularMap.png");
  // resourceManager.loadTexture2D("EmissionMap",
  // "res/textures/BOX_emissionMAP.png"); auto currTex =
  // resourceManager.getTexture2D("BOX_TEX"); auto specularTex  =
  // resourceManager.getTexture2D("SpecularMap"); auto emissionMap =
  // resourceManager.getTexture2D("EmissionMap");

  /*MATRIXES*/
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix(1.f);

  /*SHADERS*/
  auto pShaderProg = resourceManager.getShaderProgram(shaderName);
  auto pLightShader = resourceManager.getShaderProgram("LightShader");
  auto pDebugShader = resourceManager.getShaderProgram("gridShader");

  /*MAIN RENDER*/
  // Render::Renderer MainRender;
  auto MainRender = Render::RendererFactory::CreateDefault3DModelRenderer();
  // Render::createTexCubeLight(MainRender);
  Render::createTexCubeLight_Indices(MainRender);

  /*DEBUG RENDER*/
  // Render::Renderer DebugRender;
  auto DebugRender = Render::RendererFactory::CreateDebugRenderer();
  Render::createXYplane(DebugRender);

  /*GL CONTEXT*/
  /*буффер глубины*/
  glEnable(GL_DEPTH_TEST);
  /*включаю режим смешивания*/
  glEnable(GL_BLEND);
  /*задаю дефолтные настройки смешивания*/
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /*for MSAA*/
  glEnable(GL_MULTISAMPLE);

  /*CUBE properties*/
  glm::mat4 modelMatrix(1.f);
  float ang = 0.f;
  bool axisX = 0;
  bool axisY = 0;
  bool axisZ = 1;
  float shininess = 32.f;
  std::vector<glm::vec3> positionsCubes = {
      glm::vec3(-4, 3, -10), glm::vec3(0, 3, -8),  glm::vec3(0, 8, -5),
      glm::vec3(0.5, 5, -4), glm::vec3(-6, 4, -4),

      glm::vec3(-4, 3, 10),  glm::vec3(0, 3, 8),   glm::vec3(0, 8, 5),
      glm::vec3(0.5, 5, 4),  glm::vec3(-6, 4, 4)};
  std::vector<float> rotationCubes;
  for (auto& cube : positionsCubes) {
    rotationCubes.push_back(Core::Random::Float() * 360.f);
  }

  /*LIGHT properties*/
  float X = 0.031f;
  float Y = 2.697f;
  float Z = -3.047f;
  float lightAmbient = 0.20f;
  float lightDiffuse = 0.50f;
  float lightSpecular = 1.00f;
  float lightColor[3] = {1.f, 1.f, 1.f};

  auto mModel =
      resourceManager.loadModelMesh("android", "res/models/CubeNormals.glb");

  /*TIMER*/
  Core::Time MainTimer;
  while (!glfwWindowShouldClose(MainWindow.getWindow())) {
    glm::vec3 lightAmbientFactor = {lightAmbient, lightAmbient, lightAmbient};
    glm::vec3 lightDiffuseFactor = {lightDiffuse, lightDiffuse, lightDiffuse};
    glm::vec3 lightSpecularFactor = {lightSpecular, lightSpecular,
                                     lightSpecular};

    /*timer STOP*/
    /*deltaTime in sec*/
    auto deltaTime = MainTimer.stop() / 1000.f;
    /*timer START*/
    MainTimer.start();

    /*move camera*/
    MainCamera.moveCamera(deltaTime);

    projectionMatrix = MainCamera.getProjMat();
    viewMatrix = MainCamera.getViewMat();
    cameraPosition = MainCamera.getPosition();

    /*очищаю передний буфер*/
    /*glClearColor(156.f / 255.f, 156.f / 255.f, 156.f / 255.f, 1.0f);*/
    glClearColor(0 / 255.f, 0 / 255.f, 0 / 255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*ОТРИСОВКА ТОЛЬКО ЛИНИЙ МОДЕЛИ*/
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    /*ВЕРНУТЬ НА ОТРИСОВКУ ПОЛИГОНОВ*/
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

    /*DRAWING CUBE*/
    pShaderProg->use();
    modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -2.f));
    modelMatrix = glm::rotate(
        modelMatrix, glm::radians(ang),
        glm::vec3(static_cast<float>(axisX), static_cast<float>(axisY),
                  static_cast<float>(axisZ)));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(3.f, 3.f, 3.f));

    ///*TEXTURE*/
    // currTex->bindTexture2D(0);
    // pShaderProg->setTexUniform("material.texture", 0);

    ///*SPECULAT MAP*/
    // specularTex->bindTexture2D(1);
    // pShaderProg->setTexUniform("material.specularMap", 1);

    ///*EMISSION MAP*/
    // emissionMap->bindTexture2D(2);
    // pShaderProg->setTexUniform("material.emissionMap", 2);

    /*CUBE*/
    pShaderProg->serVec3Uniform(
        "light.lightColor",
        glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
    pShaderProg->serVec3Uniform("light.lightPos", glm::vec3(X, Y, Z));
    pShaderProg->serVec3Uniform(
        "cameraPos",
        glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));

    pShaderProg->setFloatUniform("material.shininess", shininess);

    pShaderProg->serVec3Uniform("light.ambientFactor", lightAmbientFactor);
    pShaderProg->serVec3Uniform("light.diffuseFactor", lightDiffuseFactor);
    pShaderProg->serVec3Uniform("light.specularFactor", lightSpecularFactor);

    pShaderProg->setFloatUniform("light.constantFactor", 1.f);
    pShaderProg->setFloatUniform("light.linearFactor", 0.09f);
    pShaderProg->setFloatUniform("light.quadFactor", 0.032f);

    pShaderProg->setMatrix4Uniform("projectionMatrix", projectionMatrix);
    pShaderProg->setMatrix4Uniform("modelMatrix", modelMatrix);
    pShaderProg->setMatrix4Uniform("viewMatrix", viewMatrix);

    mModel->draw(pShaderProg);
    // MainRender.drawArrays();
    // MainRender.drawElements();

    /*small cubes*/
    auto it = rotationCubes.begin();
    for (auto pos : positionsCubes) {
      modelMatrix = glm::mat4(1.f);
      modelMatrix = glm::translate(modelMatrix, pos);
      modelMatrix =
          glm::rotate(modelMatrix, glm::radians(*it), glm::vec3(1, 1, 1));
      pShaderProg->setMatrix4Uniform("modelMatrix", modelMatrix);
      // MainRender.drawArrays();
      MainRender->drawElements();
      it++;
    }

    /*DRAWING LIGHT*/
    pLightShader->use();
    modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(X, Y, Z));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

    pLightShader->serVec3Uniform(
        "lightColor", glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
    pLightShader->setMatrix4Uniform("projectionMatrix", projectionMatrix);
    pLightShader->setMatrix4Uniform("modelMatrix", modelMatrix);
    pLightShader->setMatrix4Uniform("viewMatrix", viewMatrix);
    // MainRender.drawArrays();
    MainRender->drawElements();

    /*DEBUG*/
    pDebugShader->use();
    modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(10000.f, 10000.f, 10000.f));
    pDebugShader->setMatrix4Uniform("projectionMatrix", projectionMatrix);
    pDebugShader->setMatrix4Uniform("modelMatrix", modelMatrix);
    pDebugShader->setMatrix4Uniform("viewMatrix", viewMatrix);
    DebugRender->drawArrays();

    /*User Interface*/
    Modules::GUIModule::GUIbegin();
    ImGui::Begin("Scene Editor");
    ImGui::SliderFloat("Cube rotation", &ang, -360.f, 360.f);
    ImGui::Checkbox("axis of rotation X", &axisX);
    ImGui::Checkbox("axis of rotation Y", &axisY);
    ImGui::Checkbox("axis of rotation Z", &axisZ);
    ImGui::Text("Time for frame:");
    ImGui::SameLine();
    ImGui::Text(std::to_string(deltaTime).c_str());
    ImGui::SameLine();
    ImGui::Text("sec");
    ImGui::End();

    ImGui::Begin("Light Editor");
    ImGui::SliderFloat("X", &X, -10.f, 10.f);
    ImGui::SliderFloat("Y", &Y, -10.f, 10.f);
    ImGui::SliderFloat("Z", &Z, -10.f, 10.f);
    ImGui::ColorEdit3("Light Color", lightColor);
    ImGui::End();

    ImGui::Begin("Light propertys");
    ImGui::SliderFloat("Ambient", &lightAmbient, 0.0f, 1.f);
    ImGui::SliderFloat("Diffuse", &lightDiffuse, 0.0f, 1.f);
    ImGui::SliderFloat("Specular", &lightSpecular, 0.0f, 1.f);
    ImGui::End();
    Modules::GUIModule::GUIend();

    /*update window (buffers)*/
    MainWindow.update();
  }
  return 0;
}
