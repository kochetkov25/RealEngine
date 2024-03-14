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
	
	/*static tex*/
	resourceManager.loadTexture2D("ColoredSqr", "res/textures/lambdaTex.png");

	unsigned int spriteSize = 128;

	resourceManager.loadSprite(
								"SqrSprite",
								"ColoredSqr",
								shaderName,
								spriteSize, spriteSize
							);
	auto SqrSprite = resourceManager.getSprite("SqrSprite");
	SqrSprite->setSpritePosition(
									glm::vec3(
												-static_cast<float>(spriteSize) / 2.f, 
												-static_cast<float>(spriteSize) / 2.f,
												-390.f
											 )
								);
	//SqrSprite->setSpriteRotation(-55.f, glm::vec3(1.0f,0.f,0.f));

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

	/*матрица модели*/
	glm::mat4 modelMatrix(1.f);

	/*матрица вида*/
	glm::mat4 viewMatrix(1.f);
	/*смещение камеры по Х*/
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -5.0f));


	/*шейдерная программа для отрисовки GL примитивов*/
	auto pShaderProg = resourceManager.getShaderProgram(shaderName);
	pShaderProg->use();

	pShaderProg->setTexUniform("tex", 0);

	pShaderProg->setMatrix4Uniform("projectionMatrix", projectionMatrix);
	//pShaderProg->setMatrix4Uniform("modelMatrix",      modelMatrix);
	pShaderProg->setMatrix4Uniform("viewMatrix", viewMatrix);


	/*НАСТРОЙКА КОНТЕКСТА OpenGL*/
	/*буффер глубины*/
	glEnable(GL_DEPTH_TEST);
	/*включаю режим смешивания*/
	glEnable(GL_BLEND);
	/*задаю дефолтные настройки смешивания*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*линейное размытие пикселей*/
	glEnable(GL_LINE_SMOOTH);

	float SRotation = 0.f;
	bool axisX = false;
	bool axisY = false;
	bool axisZ = true;
	float coordZ = -100.f;
	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
		/*очищаю передний буфер*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.6f, 0.69f, 0.929f, 0.4f);

		/*static sprite*/
		pShaderProg->use();
		/*static sprite*/
		SqrSprite->setSpriteRotation(
										SRotation, 
										glm::vec3(
													static_cast<float>(axisX), 
													static_cast<float>(axisY),
													static_cast<float>(axisZ)
												 )
		                           );
		SqrSprite->setSpritePosition(
										glm::vec3(
											-static_cast<float>(spriteSize) / 2.f,
											-static_cast<float>(spriteSize) / 2.f,
											coordZ
										)
		                            );
		SqrSprite->renderSprite();


		Modules::GUIModule::GUIupdate();

		ImGui::Begin("Scene Editor");
		ImGui::SliderFloat("Sprite rotation", &SRotation, 0.f, 360.f);
		ImGui::Checkbox("axis of rotation X", &axisX);
		ImGui::Checkbox("axis of rotation Y", &axisY);
		ImGui::Checkbox("axis of rotation Z", &axisZ);
		ImGui::SliderFloat("Sprite Z coord", &coordZ, -500.f, 0.f);
		ImGui::End();

		Modules::GUIModule::GUIdraw();


		/*меняю буферы местами (обновление окна)*/
		MainWindow.update();
	}
	return 0;
}