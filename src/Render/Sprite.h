/*
**  File        :	Sprite.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture2D.h"

#include "glad\glad.h"

#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\vec2.hpp"

#include <string>
#include <memory>

namespace Render{

	class Sprite{
	public:
		struct FramePars
		{
			FramePars(
						const glm::vec2 leftBotUV, 
						const glm::vec2 rightTopUV,
						const uint64_t duration
					 ) : _leftBotUV(leftBotUV),
						 _rightTopUV(rightTopUV),
						 _duration(duration)
			{}
			glm::vec2 _leftBotUV;
			glm::vec2 _rightTopUV;
			uint64_t _duration;
		};

		/*конструктор класса. Создается спрайт (создаются необходимые для отрисовки буфферы)*/
		Sprite(const std::shared_ptr<Texture2D> pTexture2D, 
			   const std::string subTextureName,
			   const std::shared_ptr<ShaderProgram> pShaderProgram,
			   const glm::vec2 &spritePosition,
			   const glm::vec2 &spriteSize,
			   const float rotation);
		/*освобождение памяти*/
		~Sprite();
		/*функция отрисовывает спрайт с учетом всех параметров*/
		void renderSprite();
		/*установка позицииц спрайта*/
		void setSpritePosition(const glm::vec2 &spritePosition);
		/*установка рзамера спрайта*/
		void setSpriteSize(const glm::vec2 &spriteSize);
		/*установка поворота спрайта*/
		void setSpriteRotation(const float rotation);

		//uint64_t getFrameDuration(const size_t frameID) const;
		//size_t getFramesCount() const;
	protected:
		std::shared_ptr<Texture2D> _pTexture2D;
		std::shared_ptr<ShaderProgram> _pShaderProgram;
		glm::vec2 _position;
		glm::vec2  _size;
		float _rotation;
		//GLuint _VAO;
		//GLuint _spriteVertexCoordsVBO;
		//GLuint _textureVertexCoordsVBO;

		std::shared_ptr<VertexBuffer> _pTexVertexVBO;
		VertexArray _VAO;

		std::vector<FramePars> _frameParams;
	};
}