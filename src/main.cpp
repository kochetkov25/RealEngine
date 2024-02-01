#include <glad/glad.h>
#include <iostream>
#include <string>
#include <cmath>
#include <GLFW/glfw3.h>
#include <glm\vec2.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
#include <chrono>

#include "Render\ShaderProgram.h"
#include "Resources\ResourceManager.h"
#include "Render\Texture2D.h"
#include "Render\Sprite.h"
#include "Render\AnimatedSprite.h"

//GLfloat point[] = {
//	-50.f, -50.f, 0.f,
//	-50.f, 50.f, 0.f,
//	50.f, 50.f, 0.f
//};
//
//GLfloat texCoords[] = {
//	0.0f, 0.0f,
//	0.0f, 1.0f,
//	1.0f, 1.0f
//};

// установка размеров окна в глобальной области
int g_windowSizeX = 640;
int g_windowSizeY = 480;

// определение колбек функции, которая будет вызываться при изменении
// размеров окна
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height){
	g_windowSizeX = width;
	g_windowSizeY = height;
	glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

// определение колбек функции, которая будет вызываться при нажатии клавиш
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}
}


GLFWwindow* GL_Init()
{
	// инициализация GLFW
	if (!glfwInit()) {
		std::cout << "Error load GLFW!" << std::endl;
		return nullptr;
	}

	// установка настроек OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// создание окна отрисовки
	GLFWwindow* pWindow;
	pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "MainWindow", NULL, NULL);

	// проверка создано окно или нет
	if (!pWindow) {
		std::cout << "Error create window!" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	// передача в GLFW созданных колбек функций
	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
	glfwSetKeyCallback(pWindow, glfwKeyCallback);

	// установка текущего окна как контекст отрисовки
	glfwMakeContextCurrent(pWindow);

	// проверка инициализации GLAD
	if (!gladLoadGL()) {
		std::cout << "Error load GLAD!" << std::endl;
		return nullptr;
	}
	else {
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;
	}

	// задание цвета буффера
	glClearColor(0.6f, 0.69f, 0.929f, 1.f);

	return pWindow;
}


int main(int argc, char** argv)
{
	/*вся основная инициализация GL библиотек и контекста*/
	auto pMainWindow = GL_Init();
	if (!pMainWindow)
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
	pAnimatedSprite->setSpritePosition(glm::vec2(g_windowSizeX / 6, g_windowSizeY / 2)); // установка позиции спрайта на экране
	
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
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSizeX), 0.f, static_cast<float>(g_windowSizeY), -100.f, 100.f);
	// установка отрисовываемой текстуры для sprite шейдерной программы
	pSpriteShaderProgram->use();
	pSpriteShaderProgram->setTexUniform("tex", 0);
	pSpriteShaderProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);


	/*===============================================================================================================================================================================*/


	auto lastTime = std::chrono::high_resolution_clock::now();
	// главный цикл отрисовки (рисуем пока главное окно не закрыто)
	while (!glfwWindowShouldClose(pMainWindow))
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		lastTime = currentTime;
		pAnimatedSprite->update(duration);
		// очистка буффера цвета
		glClear(GL_COLOR_BUFFER_BIT);

		pAnimatedSprite->renderSprite();

		glfwSwapBuffers(pMainWindow);

		glfwPollEvents();
	}
	// освободить все ресурсы GLFW
	glfwTerminate();
	return 0;
}