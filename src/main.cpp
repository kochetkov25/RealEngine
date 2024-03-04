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

	/*загрузка текстурного атласа*/
	std::string athlName = "defaultTextureAtlas";
	std::string athlPath = "res/textures/attack.png";
	unsigned int subTexWidth = 126;
	unsigned int subTexHeight = 39;
	std::vector<std::string> subTextureNames = {     
													"attack_0",
													"attack_1",
													"attack_2",
													"attack_3",
													"attack_4",
													"attack_5",
													"attack_6",
													"attack_7"
											   };
	resourceManager.loadTextureAthlas2D(                     
											std::move(athlName),
											std::move(athlPath),
											std::move(subTextureNames),
											subTexWidth,
											subTexHeight
									   );

	/*загрузка анимированного спрайта*/
	std::string spriteName = "attack";
	/* !!! */
	std::string shaderName = "DebugShader";
	/* !!! */
	unsigned int spriteWidth = 4 * subTexWidth;
	unsigned int spriteHeight = 4 * subTexHeight;
	std::string defaultState = "attack_0";
	resourceManager.loadAnimatedSprite(
										std::move(spriteName),
										std::move(athlName),
										std::move(shaderName),
										spriteWidth,
										spriteHeight,
										std::move(defaultState)
									  );

	/*задание длительности каждой текстуры*/
	std::vector<std::pair<std::string, size_t>> attackStates;
	size_t time = 100000000;
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_0", time));
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_1", 2 * time));
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_2", time));
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_3", time));
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_4", time));
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_5", 2 * time));
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_6", time));
	attackStates.emplace_back(std::pair<std::string, size_t>("attack_7", time));

	auto pAnimSprite = resourceManager.getAnimatedSprite("attack");
	pAnimSprite->insertState("attackState", std::move(attackStates));
	pAnimSprite->setState("attackState");

	pAnimSprite->setSpritePosition(glm::vec2(0.f, 0.f));

	/*матрица проекции*/
	/*перспективная*/
	const float aspect = static_cast<float>(MainWindow.getWidth() / MainWindow.getHeight());
	//glm::mat4 projectionMatrix = glm::perspective(
	//												glm::radians(45.f), 
	//												aspect, 
	//												0.1f, 
	//												500.f
	//											 );
	
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
	glm::mat4 projectionMatrix = glm::ortho(
												-static_cast<float>(MainWindow.getWidth()) / 2.f, static_cast<float>(MainWindow.getWidth()) / 2.f,
												-static_cast<float>(MainWindow.getHeight()) / 2.f, static_cast<float>(MainWindow.getHeight()) / 2.f,
												-100.f, 100.f
										   );

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

	/*таймер*/
	auto lastTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
		/*очищаю передний буфер*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pShaderProg->use();
		auto currentTime = std::chrono::high_resolution_clock::now();
		uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		lastTime = currentTime;
		pAnimSprite->update(duration);
		pAnimSprite->renderSprite();


		/*меняю буферы местами (обновление окна)*/
		MainWindow.update();
	}
	return 0;
}