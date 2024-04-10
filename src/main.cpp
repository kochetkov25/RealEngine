#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include <glm\vec2.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Resources\ResourceManager.h"

#include "Render\ShaderProgram.h"
#include "Render\Texture2D.h"
#include "Render\Sprite.h"
#include "Render\AnimatedSprite.h"
#include "Render\Window.h"
#include "Render\VertexBuffer.h"
#include "Render\VertexArray.h"

#include "Modules\GUIModule.h"
#include "Modules\Time.h"

#include <imgui.h>

#include "Render\Renderer.h"

#include "Render\Camera.h"

#include "Input\Input.h"


namespace Render
{
	extern void createRawCube(Renderer& render);
	extern void createTexCube(Renderer& render);
	extern void createTexCubeLight(Renderer& render);
}

int main(int argc, char** argv)
{
	/*создание главного окна отрисовки*/
	Render::Window MainWindow;
	if (!MainWindow.init())
		return -1;
	/*создание контекста ImGui*/
	Modules::GUIModule::onWindowCreate(MainWindow.getWindow());
	/*создание камеры*/
	Render::Camera MainCamera;
	MainCamera.setWindowSize(
								static_cast<float>(MainWindow.getHeight()),
								static_cast<float>(MainWindow.getWidth())
							);
	/*позиция камеры*/
	glm::vec3 cameraPosition(0.0f, 0.0f, 7.0f);
	MainCamera.setPosition(cameraPosition);
	MainCamera.setPlane(0.1f, 500.f);
	MainCamera.setVelocity(5.f);
	MainCamera.setProjectionMode(Render::Camera::ProjectionMode::PERSPECTIVE);
	/*создание объекта ресурсного менеджера*/
	ResourceManager resourceManager(argv[0]);
	/*загрузка шейдеров*/
	resourceManager.loadShaders();

	std::string shaderName = "SpriteShader";
	
	/*load TEX*/
	resourceManager.loadTexture2D("ColoredSqr", "res/textures/BOX.png");
	resourceManager.loadTexture2D("LambdaSqr", "res/textures/lambdaTex.png");
	auto currTex = resourceManager.getTexture2D("ColoredSqr");

	/*перспективная*/
	glm::mat4 projectionMatrix;
	/*матрица вида*/
	glm::mat4 viewMatrix(1.f);


	/*шейдерная программа для отрисовки GL примитивов*/
	auto pShaderProg  = resourceManager.getShaderProgram(shaderName);
	auto pLightShader = resourceManager.getShaderProgram("LightShader");



	Render::Renderer MainRender;
	Render::createTexCubeLight(MainRender);


	/*НАСТРОЙКА КОНТЕКСТА OpenGL*/
	/*буффер глубины*/
	glEnable(GL_DEPTH_TEST);
	/*включаю режим смешивания*/
	glEnable(GL_BLEND);
	/*задаю дефолтные настройки смешивания*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*values CUBE*/
	float ang = 0.f;
	bool axisX = 0;
	bool axisY = 0;
	bool axisZ = 1;
	float X =  2.f;
	float Y =  2.f;
	float Z = -3.f;
	float lightColor[3] = { 1.f, 1.f, 1.f };

	float materialAmbient  = 0.20f;
	float materialDiffuse  = 0.55f;
	float materialSpecular = 0.70f;

	float shininess = 32.f;

	float lightAmbient  = 0.20f;
	float lightDiffuse  = 0.50f;
	float lightSpecular = 1.00f;


	/*TIMER*/
	Core::Time MainTimer;

	/*BIND TEX*/
	currTex->bindTexture2D();
	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
		glm::vec3 ambientFactor = { materialAmbient, materialAmbient, materialAmbient };
		glm::vec3 diffuseFactor = { materialDiffuse, materialDiffuse, materialDiffuse };
		glm::vec3 specularFactor = { materialSpecular, materialSpecular, materialSpecular };

		glm::vec3 lightAmbientFactor = { lightAmbient,lightAmbient, lightAmbient };
		glm::vec3 lightDiffuseFactor = { lightDiffuse, lightDiffuse, lightDiffuse };
		glm::vec3 lightSpecularFactor = { lightSpecular, lightSpecular, lightSpecular };

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
		glClearColor(0.6f, 0.69f, 0.929f, 0.4f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*shader prog cube*/
		pShaderProg->use();
		/*DRAWING CUBE*/
		glm::mat4 modelMatrix(1.f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -2.f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(ang), glm::vec3(
																				static_cast<float>(axisX),
																				static_cast<float>(axisY), 
																				static_cast<float>(axisZ)
																            )
                                                                            );
		modelMatrix = glm::scale(modelMatrix, glm::vec3(3.f, 3.f, 3.f));

		pShaderProg->setTexUniform("tex", 0);
		pShaderProg->serVec3Uniform("light.lightColor", glm::vec3(
																lightColor[0],
																lightColor[1],
																lightColor[2]
															));
		pShaderProg->serVec3Uniform("light.lightPos",   glm::vec3(
																X,
																Y,
																Z
															));
		pShaderProg->serVec3Uniform("cameraPos", glm::vec3(
															cameraPosition.x,
															cameraPosition.y,
															cameraPosition.z
														 ));
		pShaderProg->serVec3Uniform("material.ambientFactor",   ambientFactor);
		pShaderProg->serVec3Uniform("material.diffuseFactor",   diffuseFactor);
		pShaderProg->serVec3Uniform("material.specularFactor",  specularFactor);
		pShaderProg->setFloatUniform("material.shininess",      shininess);

		pShaderProg->serVec3Uniform("light.ambientFactor", lightAmbientFactor);
		pShaderProg->serVec3Uniform("light.diffuseFactor", lightDiffuseFactor);
		pShaderProg->serVec3Uniform("light.specularFactor", lightSpecularFactor);

		pShaderProg->setMatrix4Uniform("projectionMatrix", projectionMatrix);
		pShaderProg->setMatrix4Uniform("modelMatrix", modelMatrix);
		pShaderProg->setMatrix4Uniform("viewMatrix", viewMatrix);
		MainRender.drawArrays();


		/*light shader*/
		pLightShader->use();
		/*DRAWING LIGHT*/
		modelMatrix = glm::mat4(1.f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(X, Y, Z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

		pLightShader->serVec3Uniform("lightColor", glm::vec3(
																lightColor[0],
																lightColor[1],
																lightColor[2]
															));
		pLightShader->setMatrix4Uniform("projectionMatrix", projectionMatrix);
		pLightShader->setMatrix4Uniform("modelMatrix", modelMatrix);
		pLightShader->setMatrix4Uniform("viewMatrix", viewMatrix);
		MainRender.drawArrays();

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

		ImGui::Begin("Material propertys");
		ImGui::SliderFloat("Ambient",  &materialAmbient,  0.1f, 1.f);
		ImGui::SliderFloat("Diffuse",  &materialDiffuse,  0.1f, 1.f);
		ImGui::SliderFloat("Specular", &materialSpecular, 0.1f, 1.f);
		ImGui::End();

		ImGui::Begin("Light propertys");
		ImGui::SliderFloat("Ambient",  &lightAmbient,  0.1f, 1.f);
		ImGui::SliderFloat("Diffuse",  &lightDiffuse,  0.1f, 1.f);
		ImGui::SliderFloat("Specular", &lightSpecular, 0.1f, 1.f);
		ImGui::End();
		Modules::GUIModule::GUIend();


		/*меняю буферы местами (обновление окна)*/
		MainWindow.update();
	}
	return 0;
}