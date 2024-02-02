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

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"


int main(int argc, char** argv)
{
	/*создание главного окна отрисовки*/
	Render::Window MainWindow;
	if (!MainWindow.init())
		return -1;


	/*===============================================================================================================================================================================*/


	// создание объекта ресурсного менеджера
	ResourceManager resourceManager(argv[0]);

	// создание шейдерной программы для отрисовки спрайтов
	auto pSpriteShaderProgram = resourceManager.loadShederProgram("SpriteShader", "res/shaders/spriteVertexShader.txt", "res/shaders/spriteFragmentShader.txt");
	if (!pSpriteShaderProgram){
		std::cerr << "Can not create SpriteShader programm!" << std::endl;
		return -1;
	}


	/*===============================================================================================================================================================================*/


	/*загрузка в ресурсный менеджер текстурного атласа*/
	std::string athlName = "defaultTextureAtlas"; // имя под которым сохраянется текстурный атлас
	std::string athlPath = "res/textures/attack.png"; // расположение текстурного атласа
	unsigned int subTexWidth = 126; // ширина одной сабтекстуры из атласа
	unsigned int subTexHeight = 39; // высота одной сабтекстуры из атласа
	std::vector<std::string> subTextureNames = {               // имена каждой из сабтекстур
		                                         "attack_0", 
												 "attack_1", 
												 "attack_2", 
												 "attack_3", 
												 "attack_4", 
												 "attack_5",
												 "attack_6",
												 "attack_7" 
	                                           }; 
	auto pTextureAtlas = resourceManager.loadTextureAthlas2D(std::move(athlName), std::move(athlPath), std::move(subTextureNames), subTexWidth, subTexHeight);


	/*===============================================================================================================================================================================*/


	/*загрузка анимированного спрайта*/
	std::string spriteName = "attack"; // имя под которым будет сохранен анимированный спрайт
	athlName = "defaultTextureAtlas"; // используемый текстурный атлас
	std::string shaderName = "SpriteShader"; // используемый шейдер 
	unsigned int spriteWidth = 4 * subTexWidth; // ширина (в пикселях) спрайта на экране
	unsigned int spriteHeight = 4 * subTexHeight; // высоа (в пикселях) спрайта на экране
	std::string defaultState = "attack_0"; // сабтекстура устанавливаемая первой в анимации по умолчанию
	auto pAnimatedSprite = resourceManager.loadAnimatedSprite(std::move(spriteName), std::move(athlName), std::move(shaderName), spriteWidth, spriteHeight, std::move(defaultState));
	pAnimatedSprite->setSpritePosition(glm::vec2(MainWindow.getWidth() / 6, MainWindow.getHeight() / 2)); // установка позиции спрайта на экране
	
	/*установка длительности каждого кадра при анимации*/
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

	pAnimatedSprite->insertState("attackState", std::move(attackStates));
	pAnimatedSprite->setState("attackState");


	/*===============================================================================================================================================================================*/


	//задание проекционной матрицы
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(MainWindow.getWidth()), 0.f, static_cast<float>(MainWindow.getHeight()), -100.f, 100.f);
	// установка отрисовываемой текстуры для sprite шейдерной программы
	pSpriteShaderProgram->use();
	pSpriteShaderProgram->setTexUniform("tex", 0);
	pSpriteShaderProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);


	/*===============================================================================================================================================================================*/


	auto lastTime = std::chrono::high_resolution_clock::now();
	// главный цикл отрисовки (рисуем пока главное окно не закрыто)
	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT); // очищаю передний буфер

		auto currentTime = std::chrono::high_resolution_clock::now();
		uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		lastTime = currentTime;
		pAnimatedSprite->update(duration); // обновляю состояние спрайта

		pAnimatedSprite->renderSprite(); // отрисовываю спрайт

		MainWindow.update(); // меняю буферы местами
	}
	return 0;
}