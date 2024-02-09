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
	//auto pSpriteShaderProgram = resourceManager.loadShederProgram("SpriteShader", "res/shaders/spriteVertexShader.txt", "res/shaders/spriteFragmentShader.txt");
	auto pDefaultSharedProgram = resourceManager.loadShederProgram("DefaultShader", "res/shaders/vertexShader.txt", "res/shaders/fragmentShader.txt");
	if (/*(!pSpriteShaderProgram) ||*/ (!pDefaultSharedProgram))
	{
		std::cerr << "Can not create Shader Programm!" << std::endl;
		return -1;
	}


	/*===============================================================================================================================================================================*/


	///*загрузка в ресурсный менеджер текстурного атласа*/
	//std::string athlName = "defaultTextureAtlas"; // имя под которым сохраянется текстурный атлас
	//std::string athlPath = "res/textures/attack.png"; // расположение текстурного атласа
	//unsigned int subTexWidth = 126; // ширина одной сабтекстуры из атласа
	//unsigned int subTexHeight = 39; // высота одной сабтекстуры из атласа
	//std::vector<std::string> subTextureNames = {               // имена каждой из сабтекстур
	//	                                         "attack_0", 
	//											 "attack_1", 
	//											 "attack_2", 
	//											 "attack_3", 
	//											 "attack_4", 
	//											 "attack_5",
	//											 "attack_6",
	//											 "attack_7" 
	//                                           }; 
	//auto pTextureAtlas = resourceManager.loadTextureAthlas2D(std::move(athlName), std::move(athlPath), std::move(subTextureNames), subTexWidth, subTexHeight);


	/*===============================================================================================================================================================================*/


	///*загрузка анимированного спрайта*/
	//std::string spriteName = "attack"; // имя под которым будет сохранен анимированный спрайт
	//athlName = "defaultTextureAtlas"; // используемый текстурный атлас
	//std::string shaderName = "SpriteShader"; // используемый шейдер 
	//unsigned int spriteWidth = 4 * subTexWidth; // ширина (в пикселях) спрайта на экране
	//unsigned int spriteHeight = 4 * subTexHeight; // высоа (в пикселях) спрайта на экране
	//std::string defaultState = "attack_0"; // сабтекстура устанавливаемая первой в анимации по умолчанию
	//auto pAnimatedSprite = resourceManager.loadAnimatedSprite(std::move(spriteName), std::move(athlName), std::move(shaderName), spriteWidth, spriteHeight, std::move(defaultState));
	//pAnimatedSprite->setSpritePosition(glm::vec2(MainWindow.getWidth() / 6, MainWindow.getHeight() / 2)); // установка позиции спрайта на экране
	//
	///*установка длительности каждого кадра при анимации*/
	//std::vector<std::pair<std::string, size_t>> attackStates;
	//size_t time = 100000000;
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_0", time));
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_1", 2 * time));
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_2", time));
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_3", time));
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_4", time));
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_5", 2 * time));
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_6", time));
	//attackStates.emplace_back(std::pair<std::string, size_t>("attack_7", time));

	//pAnimatedSprite->insertState("attackState", std::move(attackStates));
	//pAnimatedSprite->setState("attackState");


	/*===============================================================================================================================================================================*/



	/*
	* Пример 1:
	* Использую один ВБО под координаты и цвет вершин.
	* Отрисовываю две линии в форме креста
	*/
	/*координаты и цвет*/
	std::vector<GLfloat> cordCol(
									{
										200.5f, 200.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
										400.5f, 200.0f, 0.0f,   1.0f, 0.0f, 0.0f, 0.2f,
										300.5f, 400.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,
										300.5f, 100.5f, 0.0f,   0.0f, 1.0f, 0.0f, 0.2f
									}
	                          );


	std::vector<Render::VertexBuffer::BufferElement> elements{ Render::VertexBuffer::_e_DataType::Float3, Render::VertexBuffer::_e_DataType::Float4 };

	Render::VertexBuffer VBO_Test(cordCol, elements, Render::VertexBuffer::_e_Usage::Static);
	Render::VertexArray VAO_Test;

	/*ВАО*/
	VAO_Test.addBuffer(VBO_Test);


	/*=================================================================================================================================================*/


	/*
	* Пример 2:
	* Использую два ВБО: один под координаты, другой под цвет.
	* Отрисовываю треугольник
	*/
	/*координаты*/
	std::vector<GLfloat> coordsT(
									{
										500.f, 400.f,   0.f,
										600.f, 600.f,   0.f,
										700.f, 400.f,   0.f
									}
	                            );
	std::vector<Render::VertexBuffer::BufferElement> elementCoords{ Render::VertexBuffer::_e_DataType::Float3 };
	Render::VertexBuffer VBO_Coords(coordsT, elementCoords, Render::VertexBuffer::_e_Usage::Static);

	/*цвет*/
	std::vector<GLfloat> colorsT(
									{
			                           1.0f, 0.0f, 0.0f, 0.5f,
									   1.0f, 1.0f, 0.0f, 0.3f,
									   1.0f, 0.0f, 1.0f, 0.1f
		                            }
	                            );
	std::vector<Render::VertexBuffer::BufferElement> elementColors{ Render::VertexBuffer::_e_DataType::Float4 };
	Render::VertexBuffer VBO_Colors(colorsT, elementColors, Render::VertexBuffer::_e_Usage::Static);

	/*ВАО*/
	Render::VertexArray VAO_Test2;
	VAO_Test2.addBuffer(VBO_Coords);
	VAO_Test2.addBuffer(VBO_Colors);



	/*=================================================================================================================================================*/



	/*ФИЛЬТР*/
	std::vector<GLfloat> coordsColorsFilter(
									{
										0.0f,                                        0.0f,                                         0.0f,     0.0f, 1.0f, 0.0f, 0.4f,
										0.0f,                                        static_cast<GLfloat>(MainWindow.getHeight()), 0.0f,     0.0f, 1.0f, 0.0f, 0.4f,
										static_cast<GLfloat>(MainWindow.getWidth()), 0.0f,                                         0.0f,     1.0f, 1.0f, 0.0f, 0.4f,



										0.0f,                                        static_cast<GLfloat>(MainWindow.getHeight()), 0.0f,     0.0f, 1.0f, 0.0f, 0.4f,
										static_cast<GLfloat>(MainWindow.getWidth()), static_cast<GLfloat>(MainWindow.getHeight()), 0.0f,     0.0f, 1.0f, 1.0f, 0.4f,
										static_cast<GLfloat>(MainWindow.getWidth()), 0.0f,                                         0.0f,     1.0f, 1.0f, 0.0f, 0.4f
									}
								);

	std::vector<Render::VertexBuffer::BufferElement> elementsFilter{ Render::VertexBuffer::_e_DataType::Float3, Render::VertexBuffer::_e_DataType::Float4 };
	Render::VertexBuffer VBO_Filter(coordsColorsFilter, elementsFilter, Render::VertexBuffer::_e_Usage::Static);

	Render::VertexArray VAO_Filter;

	VAO_Filter.addBuffer(VBO_Filter);



	/*=================================================================================================================================================*/



	//задание проекционной матрицы
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(MainWindow.getWidth()), 0.f, static_cast<float>(MainWindow.getHeight()), -100.f, 100.f);
	glm::mat4 modelMatrix(1.f);

	// установка отрисовываемой текстуры для sprite шейдерной программы
	//pSpriteShaderProgram->use();
	//pSpriteShaderProgram->setTexUniform("tex", 0);
	//pSpriteShaderProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);

	/*шейдерная программа для отрисовки GL примитивов*/
	pDefaultSharedProgram->use();
	pDefaultSharedProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);
	pDefaultSharedProgram->setMatrix4Uniform("modelMatrix", modelMatrix);


	/*===============================================================================================================================================================================*/


	//auto lastTime = std::chrono::high_resolution_clock::now();
	// главный цикл отрисовки (рисуем пока главное окно не закрыто)
	while (!glfwWindowShouldClose(MainWindow.getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT); // очищаю передний буфер

		glEnable(GL_BLEND); // включаю режим смешивания
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // задаю дефолтные настройки смешивания

		glLineWidth(10); // толщина линии отрисовки
		glEnable(GL_LINE_SMOOTH); // линейное размытие пикселей

		//pSpriteShaderProgram->use();
		//auto currentTime = std::chrono::high_resolution_clock::now();
		//uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		//lastTime = currentTime;
		//pAnimatedSprite->update(duration); // обновляю состояние спрайта
		//pAnimatedSprite->renderSprite(); // отрисовываю спрайт


		/*рисую две линии из одного ВБО*/
		VAO_Test.bind();

		glDrawArrays(GL_LINES, 0, 4);

		VAO_Test.unbind();


		/*рисую треугольник из двух ВБО (один под координаты, другой под цвет)*/
		VAO_Test2.bind();

		glDrawArrays(GL_TRIANGLES, 0, 3);

		VAO_Test2.unbind();


		/*рисую фильтр поверх всего*/
		VAO_Filter.bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		VAO_Filter.unbind();

		MainWindow.update(); // меняю буферы местами
	}
	return 0;
}