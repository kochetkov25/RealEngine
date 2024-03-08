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
		virtual void renderSprite();
		/*установка позицииц спрайта*/
		void setSpritePosition(const glm::vec2 &spritePosition);
		/*установка рзамера спрайта*/
		void setSpriteSize(const glm::vec2 &spriteSize);
		/*установка поворота спрайта*/
		void setSpriteRotation(const float rotation);
	protected:
		std::shared_ptr<Texture2D> _pTexture2D;
		std::shared_ptr<ShaderProgram> _pShaderProgram;
		glm::vec2 _position;
		glm::vec2  _size;
		float _rotation;
		GLuint _VAO;
		GLuint _spriteVertexCoordsVBO;
		GLuint _textureVertexCoordsVBO;
	};
}