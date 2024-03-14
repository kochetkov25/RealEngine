/*
**  File        :	Sprite.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "Sprite.h"

namespace Render
{


	/*
	* создание спрайта.
	* Текстура - просто загрузка изображения в видеопамять
	* Спрайт - настройка и отрисовка уже готового изображения
	*/
	/*============================================================*/
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture2D,
				   const std::string subTextureName,
				   std::shared_ptr<ShaderProgram> pShaderProgram,
				   const glm::vec3 &spritePosition,
				   const glm::vec2 &spriteSize,
				   const float rotation)
	{
		/*установка входных параметров*/
		_pShaderProgram = std::move(pShaderProgram);
		_pTexture2D = std::move(pTexture2D);
		_position = spritePosition;
		_size = spriteSize;
		_rotation = rotation;
		/*значение по умолчанию (-1)*/
		_lastFrameID = -1;
		_axis = glm::vec3(0.f, 0.f, 1.f);

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
	void Sprite::renderSprite(short frameId)
	{
		if (_lastFrameID != frameId)
		{
			if (frameId > _frameParams.size() || 
			    _frameParams.size() == 0)
			{
				std::cerr << "Unknown frame ID!" << std::endl;
				return;
			}

			_lastFrameID = frameId;

			const auto& currFramePars = _frameParams[frameId];
			auto subTexture = _pTexture2D->getSubTexture2D(currFramePars._name);

			/*координаты текстуры от 0 до 1*/
			std::vector<GLfloat> textureVertexCoords(
				{
					/*первый треугольник*/
					subTexture._leftBottomUV.x,  subTexture._leftBottomUV.y,
					subTexture._leftBottomUV.x,  subTexture._rightTopUV.y,
					subTexture._rightTopUV.x, subTexture._rightTopUV.y,
					/*второй треугольник*/
					subTexture._rightTopUV.x, subTexture._rightTopUV.y,
					subTexture._rightTopUV.x, subTexture._leftBottomUV.y,
					subTexture._leftBottomUV.x,  subTexture._leftBottomUV.y
				}
			);
			_pTexVertexVBO->updateData(textureVertexCoords);
		}

		_pShaderProgram->use();


		/*
		* поскольку результат перемножения матриц 
		* выполняется в обратном порядке (лин. ал.)
		* следующие комментарии следует читать снизу вверх
		*/
		glm::mat4 modelMatrix(1.f);
		/*перемещаем спрайт в нужную нам точку мировой ск*/
		modelMatrix = glm::translate(modelMatrix, _position);
		/*перемещаем спрайт в первоночальное положение*/
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f*_size.x, 0.5*_size.y, 0.f));
		/*поворачиваем спрайт на нужный угол*/
		modelMatrix = glm::rotate(modelMatrix, glm::radians(_rotation), _axis);
		/*
		* координаты спрайта задаются относительно нижнего левого угла,
		* поэтому, чтобы повернуть спрайт вокруг своей оси,
		* перемещеаем центр квадратного спрайта в точку (0, 0)
		*/
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * _size.x, -0.5 * _size.y, 0.f));
		/*
		* скалирование матрицы по трем осям.
		* Берем спрайт 1 на 1 и скалируется до нужного размера
		*/
		modelMatrix = glm::scale(modelMatrix, glm::vec3(_size, 1.f));


		_VAO.bind();
		/*устанавливаем модельную матрицу*/
		_pShaderProgram->setMatrix4Uniform("modelMatrix", modelMatrix);
		/*активируем нулевой слот для текстур*/
		glActiveTexture(GL_TEXTURE0);
		/*делаем переданную текстуру активной*/
		_pTexture2D->bindTexture2D();

		/*установка параметров для прозрачности*/
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/*активация режима смешивания*/
		glEnable(GL_BLEND);

		/*отрисовываем спрайт*/
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*деактивируем смешивание*/
		glDisable(GL_BLEND);

		/*отвязываем объект вершинного массива*/
		_VAO.unbind();
		
	}

	/*============================================================*/
	/*позиция спрайта*/
	void Sprite::setSpritePosition(const glm::vec3 &spritePosition)
	{
		_position = spritePosition;
	}

	/*============================================================*/
	/*размер спрайта*/
	void Sprite::setSpriteSize(const glm::vec2 &spriteSize)
	{
		_size = spriteSize;
	}

	/*============================================================*/
	/*поворот спрайта*/
	void Sprite::setSpriteRotation(const float rotationAng, const glm::vec3& axis)
	{
		_rotation = rotationAng;
		_axis = axis;
	}

	/*============================================================*/
	/*получить длительность кадра*/
	uint64_t Sprite::getFrameDuration(const size_t frameID) const
	{
		if (frameID < _frameParams.size())
			return _frameParams[frameID]._duration;

		std::cerr << "sprite with this ID does not exist." << std::endl;
		return 0;
	}

	/*============================================================*/
	/*получить кол-во кадров*/
	size_t Sprite::getFramesCount() const
	{
		return _frameParams.size();
	}

	/*============================================================*/
	/*установка параметров для каждого кадра анимации*/
	void Sprite::setAnimParams(
								std::vector<std::pair<std::string, uint64_t>>& framesDurations
							  )
	{
		for (auto& frame : framesDurations)
			_frameParams.emplace_back(FramePars(frame.first, frame.second));
	}


}