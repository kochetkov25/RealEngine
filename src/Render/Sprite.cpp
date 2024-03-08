/*
**  File        :	Sprite.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "Sprite.h"

namespace Render{


	/*
	* создание спрайта.
	* Текстура - просто загрузка изображения в видеопамять
	* Спрайт - настройка и отрисовка уже готового изображения
	*/
	/*============================================================*/
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture2D,
				   const std::string subTextureName,
				   const std::shared_ptr<ShaderProgram> pShaderProgram,
				   const glm::vec2 &spritePosition,
				   const glm::vec2 &spriteSize,
				   const float rotation)
	{
		/* установка входных параметров*/
		_pShaderProgram = std::move(pShaderProgram);
		_pTexture2D = std::move(pTexture2D);
		_position = spritePosition;
		_size = spriteSize;
		_rotation = rotation;

		/*координаты спрайта от 0 до 1*/
		std::vector<GLfloat> spriteVertexCoords(
													{
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
													}
											  );

		/*получение текстуры*/
		auto subTexture = _pTexture2D->getSubTexture2D(subTextureName);

		/*координаты текстуры от 0 до 1*/
		std::vector<GLfloat> textureVertexCoords(
													{
														// первый треугольник
														subTexture._leftBottomUV.x, subTexture._leftBottomUV.y,
														subTexture._leftBottomUV.x, subTexture._rightTopUV.y,
														subTexture._rightTopUV.x, subTexture._rightTopUV.y,
														// второй треугольник
														subTexture._rightTopUV.x, subTexture._rightTopUV.y,
														subTexture._rightTopUV.x, subTexture._leftBottomUV.y,
														subTexture._leftBottomUV.x, subTexture._leftBottomUV.y
													}
											    );

		std::vector<VertexBuffer::BufferElement> elementsSprite{ Render::VertexBuffer::_e_DataType::Float2 };
		VertexBuffer spriteCoordsVBO(spriteVertexCoords, elementsSprite, VertexBuffer::_e_Usage::Static);
		_VAO.addBuffer(spriteCoordsVBO);

		std::vector<VertexBuffer::BufferElement> elementsTex{ Render::VertexBuffer::_e_DataType::Float2 };
		_pTexVertexVBO = std::make_shared<VertexBuffer>(textureVertexCoords, elementsTex, VertexBuffer::_e_Usage::Static);
		_VAO.addBuffer(*_pTexVertexVBO.get());
	}

	/*============================================================*/
	/*деструктор*/
	Sprite::~Sprite()
	{
	}

	/*============================================================*/
	/*отрисовка спрайта*/
	void Sprite::renderSprite()
	{
		_pShaderProgram->use();

		// модельная матрица
		glm::mat4 modelMatrix(1.f);

		/*
		* поскольку результат перемножения матриц 
		* выполняется в обратном порядке (лин. ал.)
		* следующие комментарии следует читать снизу вверх
		*/

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


		_VAO.bind();
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
		_VAO.unbind();
		
		// деактивируем смешивание
		glDisable(GL_BLEND);
	}

	/*============================================================*/
	void Sprite::setSpritePosition(const glm::vec2 &spritePosition)
	{
		_position = spritePosition;
	}

	/*============================================================*/
	void Sprite::setSpriteSize(const glm::vec2 &spriteSize)
	{
		_size = spriteSize;
	}

	/*============================================================*/
	void Sprite::setSpriteRotation(const float rotation)
	{
		_rotation = rotation;
	}

	///*============================================================*/
	//uint64_t Sprite::getFrameDuration(const size_t frameID) const
	//{
	//	if (frameID < _frameParams.size())
	//		return _frameParams[frameID]._duration;

	//	std::cerr << "sprite with this ID does not exist." << std::endl;
	//	return 0;
	//}

	///*============================================================*/
	//size_t Sprite::getFramesCount() const
	//{
	//	return _frameParams.size();
	//}

}