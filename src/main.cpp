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

	// создание объекта ресурсного менеджера
	ResourceManager resourceManager(argv[0]);

	// создание шейдерной программы для отрисовки спрайтов
	auto pDefaultSharedProgram = resourceManager.loadShederProgram(
																	"DefaultShader",
																	"res/shaders/vertexShader.txt",
																	"res/shaders/fragmentShader.txt"
																  );
	if ((!pDefaultSharedProgram))
	{
		std::cerr << "Can not create Shader Programm!" << std::endl;
		return -1;
	}


	/*матрица проекции*/
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), 1024.f / 768.f, 0.1f, 500.f);
	//glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(MainWindow.getWidth()), 0.f, static_cast<float>(MainWindow.getHeight()), -100.f, 100.f);
	//glm::mat4 projectionMatrix = glm::ortho(
	//	                                     -static_cast<float>(MainWindow.getWidth()) / 2.f,  static_cast<float>(MainWindow.getWidth()) / 2.f,
	//	                                     -static_cast<float>(MainWindow.getHeight()) / 2.f, static_cast<float>(MainWindow.getHeight()) / 2.f,
	//	                                     -100.f, 100.f
	//                                       );



	/*матрица модели*/
	glm::mat4 modelMatrix(1.f);
	/*??? поворот*/
	auto ang_rad = glm::radians(0.f);
	modelMatrix = glm::rotate(modelMatrix, ang_rad, glm::vec3(1.0f, 0.0f, 0.0f));

	/*матрица вида*/
	glm::mat4 viewMatrix(1.f);
	/*смещение камеры по Х*/
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));


	/*шейдерная программа для отрисовки GL примитивов*/
	pDefaultSharedProgram->use();
	pDefaultSharedProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);
	pDefaultSharedProgram->setMatrix4Uniform("modelMatrix", modelMatrix);
	pDefaultSharedProgram->setMatrix4Uniform("viewMatrix", viewMatrix);


	/*задание координа с помощбю рендера*/
	Render::Renderer mainRenderer;

	const float zCoord = -400.f;

	mainRenderer.begin(GL_TRIANGLES);

	mainRenderer.verex3(0.f, 0.f, zCoord);
	mainRenderer.color4(255.f, 0.f, 0.f, 0.5f);

	mainRenderer.verex3(200.f, 0.f, zCoord);
	mainRenderer.color4(255, 0, 0);

	mainRenderer.verex3(100.f, 200.f, zCoord);
	mainRenderer.color4(255, 0, 0);

	mainRenderer.end();

	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
		/*очищаю передний буфер*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		/*включаю режим смешивания*/
		glEnable(GL_BLEND);
		/*задаю дефолтные настройки смешивания*/
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/*толщина линии отрисовки*/
		glLineWidth(10);
		/*линейное размытие пикселей*/
		glEnable(GL_LINE_SMOOTH);


		/*отрисовка элементов*/
		mainRenderer.drawArrays();


		/*меняю буферы местами (обновление окна)*/
		MainWindow.update();
	}
	return 0;
}