#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <string>

namespace Render{

	// создание спрайта.
	// Текстура - просто загрузка изображения в видеопамять
	// Спрайт - настройка и отрисовка уже готового изображения
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture2D,
		const std::string subTextureName,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2 &spritePosition,
		const glm::vec2 &spriteSize,
		const float rotation)
	{
		// установка входных параметров
		_pShaderProgram = std::move(pShaderProgram);
		_pTexture2D = std::move(pTexture2D);
		_position = spritePosition;
		_size = spriteSize;
		_rotation = rotation;

		// координаты спрайта от 0 до 1
		const GLfloat spriteVertexCoords[] = {
			//  2--3    1
			//  | /   / |
			//  1    3--2

			// первый треугольник
			0.f, 0.f,
			0.f, 1.f,
			1.f,1.f,
			// второй треугольник
			1.f,1.f,
			1.f,0.f,
			0.f,0.f
		};

		// !!! ТОНКИЙ МОМЕНТ !!! функция ниже, вернет сабтекстуру по имени, если таковая есть.
		// если такой сабтекстуры нет, будет возвращена вся текстура, что позовляет создавать как спрайты целых текстур, так и спрайты сабтекстур
		auto subTexture = _pTexture2D->getSubTexture2D(subTextureName);

		// координаты текстуры от 0 до 1
		const GLfloat textureVertexCoords[] = {
			// первый треугольник
			subTexture._leftBottomUV.x, subTexture._leftBottomUV.y,
			subTexture._leftBottomUV.x, subTexture._rightTopUV.y,
			subTexture._rightTopUV.x, subTexture._rightTopUV.y,
			// второй треугольник
			subTexture._rightTopUV.x, subTexture._rightTopUV.y,
			subTexture._rightTopUV.x, subTexture._leftBottomUV.y,
			subTexture._leftBottomUV.x, subTexture._leftBottomUV.y
		};
		// создаем объект вершинного массива
		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		// процесс создания буффера для координат вершин спрайта
		glGenBuffers(1, &_spriteVertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _spriteVertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertexCoords), &spriteVertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// процесс создания буффера для координат вершин текстуры
		glGenBuffers(1, &_textureVertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _textureVertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertexCoords), &textureVertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// отвязываем буфферы и оставляем контекст OpenGl в нулевом положении
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Sprite::~Sprite()
	{
		// осовобождаем память видеокарты
		glDeleteBuffers(1, &_spriteVertexCoordsVBO);
		glDeleteBuffers(1, &_textureVertexCoordsVBO);
		glDeleteVertexArrays(1, &_VAO);
	}

	void Sprite::renderSprite()
	{
		_pShaderProgram->use();

		// модельная матрица
		glm::mat4 modelMatrix(1.f);

		// поскольку результат перемножения матриц выполняется в обратном порядке
		// следующие комментариис следует читать снизу вверх

		// перемещаем спрайт в нужную нам точку мировой ск
		modelMatrix = glm::translate(modelMatrix, glm::vec3(_position, 0.f));
		// перемещаем спрайт в первоночальное положение
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f*_size.x, 0.5*_size.y, 0.f));
		// поворачиваем спрайт на нужный угол
		modelMatrix = glm::rotate(modelMatrix, glm::radians(_rotation), glm::vec3(0.f, 0.f, 1.f));
		// как известно координаты спрайта задаются относительно нижнего левого угла, поэтому
		// чтобы повернуть спрайт вокруг своей оси, перемещеаем центр квадратного спрайта в точку 0 0
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f*_size.x, -0.5*_size.y, 0.f));
		// скалирование матрицы по трем осям. Берется спрайт 1 на 1 и скалируется до нужного размера
		modelMatrix = glm::scale(modelMatrix, glm::vec3(_size, 1.f));


		glBindVertexArray(_VAO);
		// устанавливаем модельную матрицу
		_pShaderProgram->setMatrix4Uniform("modelMatrix", modelMatrix);
		// активируем нулевой слот для текстур
		glActiveTexture(GL_TEXTURE0);
		// делаем переданную текстуру активной
		_pTexture2D->bindTexture2D();

		// установка параметров для прозрачности
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// активация смешивания
		glEnable(GL_BLEND);

		// отрисовываем спрайт
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// отвязываем объект вершинного массива
		glBindVertexArray(0);
		
		// деактивируем смешивание
		glDisable(GL_BLEND);
	}
	void Sprite::setSpritePosition(const glm::vec2 &spritePosition)
	{
		_position = spritePosition;
	}
	void Sprite::setSpriteSize(const glm::vec2 &spriteSize)
	{
		_size = spriteSize;
	}
	void Sprite::setSpriteRotation(const float rotation)
	{
		_rotation = rotation;
	}
}