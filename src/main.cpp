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

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

#include "Render\Renderer.h"


int main(int argc, char** argv)
{
	/*создание главного окна отрисовки*/
	Render::Window MainWindow;
	if (!MainWindow.init())
		return -1;

	/*создание объекта ресурсного менеджера*/
	ResourceManager resourceManager(argv[0]);

	/*создание шейдерных программ*/
	/*для обычной отрисовки примитивов*/
	resourceManager.loadShederProgram(
										"DefaultShader",
										"res/shaders/vertexShader.txt",
										"res/shaders/fragmentShader.txt"
									 );
	/*для отрисовки спрайтов*/
	resourceManager.loadShederProgram(
										"SpriteShader",
										"res/shaders/spriteVertexShader.txt",
										"res/shaders/spriteFragmentShader.txt"
									 );
	/*для отладки*/
	resourceManager.loadShederProgram(
										"DebugShader",
										"res/shaders/spriteVertexShader.txt",
										"res/shaders/DebugFragmentShader.txt"
									 );

	std::string shaderName = "SpriteShader";
	


	std::string athlName = "defaultTextureAtlas";     // name for tex athlas
	std::string athlPath = "res/textures/attack.png"; // path of tex athlas

	unsigned int subTexWidth = 126; 				  // width of subtex
	unsigned int subTexHeight = 39; 				  // height of subtex

	std::vector<std::string> subTextNames = {      // names of every subtex
												 "attack_0",
												 "attack_1",
												 "attack_2",
												 "attack_3",
												 "attack_4",
												 "attack_5",
												 "attack_6",
												 "attack_7"
										   };
	/*duration of every frame is 100000000 msec*/
	std::vector<std::pair<std::string, uint64_t>> FramesDurations;
	for (auto& name : subTextNames)
		FramesDurations.emplace_back(std::pair<std::string, uint64_t>(name, 100000000));
	/*load tex athlas into resourceManager*/
	resourceManager.loadTextureAthlas2D(
										std::move(athlName),
										std::move(athlPath),
										std::move(subTextNames),
										subTexWidth,
										subTexHeight
									   );
	/*load sprite into res manager*/
	resourceManager.loadSprite(
								"TestSprite", 
								"defaultTextureAtlas", 
								shaderName,
								250, 100
							  );
	auto TestSprite = resourceManager.getSprite("TestSprite");
	TestSprite->setAnimParams(FramesDurations);
	TestSprite->setSpritePosition(glm::vec3(-200.f, 0.f, -199.f));
	/*anim sprite*/
	Render::AnimatedSprite TestAnim(TestSprite);

	/*static tex*/
	resourceManager.loadTexture2D("ColoredSqr", "res/textures/firstTex.png");

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
												-490.f
											 )
								);

	/*матрица проекции*/
	/*перспективная*/
	//const float aspect = static_cast<float>(MainWindow.getWidth() / MainWindow.getHeight());
	const float aspect = 1024.f / 768.f;
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
	//pShaderProg->setMatrix4Uniform("viewMatrix",       viewMatrix);



	/*НАСТРОЙКА КОНТЕКСТА OpenGL*/
	/*буффер глубины*/
	glEnable(GL_DEPTH_TEST);
	/*включаю режим смешивания*/
	glEnable(GL_BLEND);
	/*задаю дефолтные настройки смешивания*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*линейное размытие пикселей*/
	glEnable(GL_LINE_SMOOTH);

	auto lastTime = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
		/*очищаю передний буфер*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto currentTime = std::chrono::high_resolution_clock::now();
		uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		lastTime = currentTime;

		//pShaderProg->use();
		//TestAnim.update(duration);
		//TestAnim.render();

		/*static sprite*/
		pShaderProg->use();
		/*static sprite*/
		SqrSprite->renderSprite();

		/*меняю буферы местами (обновление окна)*/
		MainWindow.update();
	}
	return 0;
}