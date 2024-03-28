#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <chrono>

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

#include "Modules/GUIModule.h"

#include <imgui.h>

#include "Render\Renderer.h"


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
	/*создание объекта ресурсного менеджера*/
	ResourceManager resourceManager(argv[0]);
	/*загрузка шейдеров*/
	resourceManager.loadShaders();

	std::string shaderName = "SpriteShader";
	
	/*load TEX*/
	resourceManager.loadTexture2D("ColoredSqr", "res/textures/lambdaTex.png");
	auto currTex = resourceManager.getTexture2D("ColoredSqr");

	/*матрица проекции*/
	/*перспективная*/
	const float aspect = static_cast<float>(MainWindow.getWidth()) /
						 static_cast<float>(MainWindow.getHeight());
	glm::mat4 projectionMatrix = glm::perspective(
													glm::radians(45.f), 
													aspect, 
													0.1f, 
													500.f
												 );
	
	/*ортогональная, центр - левый нижний угол*/
	//glm::mat4 projectionMatrix = glm::ortho(
	//											0.f,
	//											static_cast<float>(MainWindow.getWidth()), 
	//											0.f, 
	//											static_cast<float>(MainWindow.getHeight()), 
	//											-100.f, 
	//											100.f
	//									   );

	/*ортогональная, центр - центральная точка окна отрисовки*/
	//glm::mat4 projectionMatrix = glm::ortho(
	//											-static_cast<float>(MainWindow.getWidth()) / 2.f, static_cast<float>(MainWindow.getWidth()) / 2.f,
	//											-static_cast<float>(MainWindow.getHeight()) / 2.f, static_cast<float>(MainWindow.getHeight()) / 2.f,
	//											-500.f, 500.f
	//									   );

	 
	/*матрица вида*/
	glm::mat4 viewMatrix(1.f);
	/*позиция камеры*/
	glm::vec3 cameraPosition(0.0f, -2.0f, -7.0f);
	viewMatrix = glm::translate(viewMatrix, cameraPosition);


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

	float ambientFactor = 0.1f;
	float diffuseFactor = 0.1f;
	float specularFactor = 0.1f;
	float shininess = 32.f;

	/*BIND TEX*/
	currTex->bindTexture2D();
	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
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
		pShaderProg->serVec3Uniform("lightColor", glm::vec3(
																lightColor[0],
																lightColor[1],
																lightColor[2]
															));
		pShaderProg->serVec3Uniform("lightPos",   glm::vec3(
																X,
																Y,
																Z
															));
		pShaderProg->serVec3Uniform("cameraPos", glm::vec3(
															-cameraPosition.x,
															-cameraPosition.y,
															-cameraPosition.z
														 ));
		pShaderProg->setFloatUniform("ambientFactor",  ambientFactor);
		pShaderProg->setFloatUniform("diffuseFactor",  diffuseFactor);
		pShaderProg->setFloatUniform("specularFactor", specularFactor);
		pShaderProg->setFloatUniform("shininess",      shininess);
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
		Modules::GUIModule::GUIupdate();
		ImGui::Begin("Scene Editor");
		ImGui::SliderFloat("Cube rotation", &ang, -360.f, 360.f);
		ImGui::Checkbox("axis of rotation X", &axisX);
		ImGui::Checkbox("axis of rotation Y", &axisY);
		ImGui::Checkbox("axis of rotation Z", &axisZ);
		ImGui::End();

		ImGui::Begin("Light Editor");
		ImGui::SliderFloat("X", &X, -10.f, 10.f);
		ImGui::SliderFloat("Y", &Y, -10.f, 10.f);
		ImGui::SliderFloat("Z", &Z, -10.f, 10.f);
		ImGui::ColorEdit3("Light Color", lightColor);
		ImGui::SliderFloat("Ambient Factor", &ambientFactor, 0.f, 1.f);
		ImGui::SliderFloat("Diffuse Factor", &diffuseFactor, 0.f, 1.f);
		ImGui::SliderFloat("Specular Factor", &specularFactor, 0.f, 1.f);
		ImGui::SliderFloat("Shininess", &shininess, 2.f, 512.f);
		ImGui::End();
		Modules::GUIModule::GUIdraw();


		/*меняю буферы местами (обновление окна)*/
		MainWindow.update();
	}
	return 0;
}