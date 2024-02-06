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

	GLfloat firstTriangle[] = {
		 100.5f, 50.0f, 0.0f,
		 200.5f, 50.0f, 0.0f,
		 200.5f, 200.5f, 0.0f,
		 400.5f, 400.5f, 0.0f
	};

	GLfloat secondTriangle[] = {
	     200.5f, 100.0f,  0.0f,
	     400.5f, 100.0f,  0.0f,
	     450.5f, 450.5f,  0.0f,
		 800.5f, 600.5f,  0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 0.5f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.5f
	};

	GLfloat lines_colors[] = {
	     200.5f, 200.0f,  0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
		 400.5f, 200.0f,  0.0f,    1.0f, 0.0f, 0.0f, 0.2f,
		 300.5f, 400.5f,  0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
		 300.5f, 100.5f,  0.0f,    0.0f, 1.0f, 0.0f, 0.2f
	};

	GLuint VBO[3], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(3, VBO);


	/* один ВАО под первые 2 линии (не обращать внимание, что называется triangle) */
	//glBindVertexArray(VAO[0]);

	//	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//	glEnableVertexAttribArray(0);

	//	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//	glEnableVertexAttribArray(1);

	//glBindVertexArray(0);


	/* второй ВАО под вторые  2 линии (не обращать внимание, что называется triangle) */
	//glBindVertexArray(VAO[1]);

	//	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//	glEnableVertexAttribArray(0);

	//	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//	glEnableVertexAttribArray(1);

	//glBindVertexArray(0);



	/* один ВБО, содержащий положения точек и цвет */
	glBindVertexArray(VAO[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // делаю буффер ВБО активным
		glBufferData(GL_ARRAY_BUFFER, sizeof(lines_colors), lines_colors, GL_STATIC_DRAW); // перемещаю данные из оперативной память в память видеокарты

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0); // сообщаю OpenGL как нужно воспринимать полученные данные
		                                                                                  // одна вершина содержит 7 параметров (x,y,z,r,g,b,a);
		                                                                                  // в location с номером 0 пойдут x y z поэтому для них смещение 0 (offset)
		glEnableVertexAttribArray(0); // настройки задаются для location с номером 0

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // в location с номером 1 пойдет r g b a, для них смещение (offset) 3 ,
		                                                                                                      // потому что первые 3 это x y z
		glEnableVertexAttribArray(1); // настройки задаются для location с номером 0

	glBindVertexArray(0);



	//задание проекционной матрицы
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(MainWindow.getWidth()), 0.f, static_cast<float>(MainWindow.getHeight()), -100.f, 100.f);
	glm::mat4 modelMatrix(1.f);
	// установка отрисовываемой текстуры для sprite шейдерной программы
	//pSpriteShaderProgram->use();
	//pSpriteShaderProgram->setTexUniform("tex", 0);
	//pSpriteShaderProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);

	pDefaultSharedProgram->use();
	pDefaultSharedProgram->setMatrix4Uniform("projectionMatrix", projectionMatrix);
	pDefaultSharedProgram->setMatrix4Uniform("modelMatrix", modelMatrix);


	/*===============================================================================================================================================================================*/


	auto lastTime = std::chrono::high_resolution_clock::now();
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

		/*рисую 4 линии из из двух разных ВАО*/
		//glBindVertexArray(VAO[0]);
		//glDrawArrays(GL_LINES, 0, 4);

		//glBindVertexArray(VAO[1]);
		//glDrawArrays(GL_LINES, 0, 4);



		/*рисую две линии из одного ВАО*/
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_LINES, 0, 4);

		glBindVertexArray(0); // отвязываем ВАО

		MainWindow.update(); // меняю буферы местами
	}
	return 0;
}