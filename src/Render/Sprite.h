#pragma once

#include <memory>
#include "glad\glad.h"
#include "glm\vec2.hpp"
#include <string>


namespace Render{
	class Texture2D;
	class ShaderProgram;

	class Sprite{
	public:
		// конструктор класса. Создается спрайт (создаются необходимые для отрисовки буфферы)
		Sprite(const std::shared_ptr<Texture2D> pTexture2D, 
			const std::string subTextureName,
			const std::shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2 &spritePosition,
			const glm::vec2 &spriteSize,
			const float rotation);
		// освобождение памяти
		~Sprite();
		// функция отрисовывает спрайт с учетом всех параметров
		virtual void renderSprite();
		// установка позицииц спрайта
		void setSpritePosition(const glm::vec2 &spritePosition);
		// установка рзамера спрайта
		void setSpriteSize(const glm::vec2 &spriteSize);
		// установка поворота спрайта
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