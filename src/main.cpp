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

GLfloat point[] = {
	-50.f, -50.f, 0.f,
	-50.f, 50.f, 0.f,
	50.f, 50.f, 0.f
};

GLfloat texCoords[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
};

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

int main(int argc, char** argv){
	// инициализация GLFW
	if (!glfwInit()){
		std::cout << "Error load GLFW!" << std::endl;
		return -1;
	}

	// установка настроек OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// создание окна отрисовки
	GLFWwindow* pWindow;
	pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "MainWindow", NULL, NULL);

	// проверка создано окно или нет
	if (!pWindow){
		std::cout << "Error create window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// передача в GLFW созданных колбек функций
	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
	glfwSetKeyCallback(pWindow, glfwKeyCallback);

	// установка текущего окна как контекст отрисовки
	glfwMakeContextCurrent(pWindow);

	// проверка инициализации GLAD
	if (!gladLoadGL()){
		std::cout << "Error load GLAD!" << std::endl;
		return -1;
	}
	else{
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;
	}

	// задание цвета буффера
	glClearColor(0.6f, 0.69f, 0.929f, 1.f);

	// создание объекта ресурсного менеджера
	ResourceManager resourceManager(argv[0]);

	// создание шейдерной программы для отрисовки спрайтов
	auto pSpriteShaderProgram = resourceManager.loadShederProgram("SpriteShader", "res/shaders/spriteVertexShader.txt", "res/shaders/spriteFragmentShader.txt");
	if (!pSpriteShaderProgram){
		std::cerr << "Can not create SpriteShader programm!" << std::endl;
		return -1;
	}


	// создание вектора, который содержит имена всех саб текстур
	std::vector<std::string> subTextureNames = { "KnightAnim_0", "KnightAnim_1", "KnightAnim_2", "KnightAnim_3", "KnightAnim_4", "KnightAnim_5"};

	// загрузка в ресурсный менеджер текстурного атласа (загружается текстура в ресурсный менеджер, а также загружаются все координаты саб текстур)
	auto pTextureAtlas = resourceManager.loadTextureAthlas2D("defaultTextureAtlas", "res/textures/KnightAnim.png", 
															  std::move(subTextureNames), 142, 94);

	// загрузка анимированного спрайта
	auto pAnimatedSprite = resourceManager.loadAnimatedSprite("KnightAnim", "defaultTextureAtlas", "SpriteShader", 150, 150, "KnightAnim_0");
	pAnimatedSprite->setSpritePosition(glm::vec2(g_windowSizeX / 2, g_windowSizeY / 2));

	std::vector<std::pair<std::string, size_t>> knightStates;

	size_t time = 100000000;
	knightStates.emplace_back(std::pair<std::string, size_t>("KnightAnim_0", time));
	knightStates.emplace_back(std::pair<std::string, size_t>("KnightAnim_1", time*2));
	knightStates.emplace_back(std::pair<std::string, size_t>("KnightAnim_2", time));
	knightStates.emplace_back(std::pair<std::string, size_t>("KnightAnim_3", time*2));
	knightStates.emplace_back(std::pair<std::string, size_t>("KnightAnim_4", time));
	knightStates.emplace_back(std::pair<std::string, size_t>("KnightAnim_5", time));

	pAnimatedSprite->insertState("knightState", std::move(knightStates));
	pAnimatedSprite->setState("knightState");

	//задание проекционной матрицы
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSizeX), 0.f, static_cast<float>(g_windowSizeY), -100.f, 100.f);

	// установка отрисовываемой текстуры для sprite шейдерной программы
	pSpriteShaderProgram->use();
	pSpriteShaderProgram->setTexUniform("tex", 0);
	pSpriteShaderProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);


	auto lastTime = std::chrono::high_resolution_clock::now();
	// главный цикл отрисовки (рисуем пока главное окно не закрыто)
	while (!glfwWindowShouldClose(pWindow))
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		lastTime = currentTime;
		pAnimatedSprite->update(duration);
		// очистка буффера цвета
		glClear(GL_COLOR_BUFFER_BIT);

		pAnimatedSprite->renderSprite();

		glfwSwapBuffers(pWindow);

		glfwPollEvents();
	}
	// освободить все ресурсы GLFW
	glfwTerminate();
	return 0;
}