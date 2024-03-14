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

namespace Render
{


	class Sprite{
	public:
		/*
		* Для анимации спрайта.
		* Структура описывает имя и длительность
		* каждого кадра
		*/
		struct FramePars
		{
			FramePars(
						const std::string name,
						const uint64_t duration
					 ) : _name(name),
				         _duration(duration)
			{}
			std::string _name;
			uint64_t _duration;
		};

		/*конструктор класса*/
		Sprite(std::shared_ptr<Texture2D> pTexture2D, 
			   const std::string subTextureName,
			   std::shared_ptr<ShaderProgram> pShaderProgram,
			   const glm::vec3 &spritePosition,
			   const glm::vec2 &spriteSize,
			   const float rotation);

		/*деструктор*/
		~Sprite();

		/*
		* функция отрисовки спрайта.
		* frameId для анимации. По умолчанию задан невалидным
		*/
		void renderSprite(short frameId = -1);

		/*установка позицииц спрайта*/
		void setSpritePosition(const glm::vec3 &spritePosition);

		/*установка рзамера спрайта*/
		void setSpriteSize(const glm::vec2 &spriteSize);

		/*установка поворота спрайта*/
		void setSpriteRotation(const float rotationAng, const glm::vec3& axis = glm::vec3(0.f,0.f,1.f));

		/*получить длительность текущего кадра*/
		uint64_t getFrameDuration(const size_t frameID) const;

		/*получить кол-во кадров в анимации*/
		size_t getFramesCount() const;

		/*установка параметров каждого кадра анимации*/
		void setAnimParams(std::vector<std::pair<std::string, uint64_t>>& framesDurations);

	protected:
		std::shared_ptr<Texture2D> _pTexture2D;
		std::shared_ptr<ShaderProgram> _pShaderProgram;
		glm::vec3 _position;
		glm::vec2  _size;
		glm::vec3 _axis;
		float _rotation;

		/*VAO и VBO*/
		std::shared_ptr<VertexBuffer> _pTexVertexVBO;
		VertexArray _VAO;

		std::vector<FramePars> _frameParams;

		/*параметр для кэширования кадра*/
		 short _lastFrameID;
	};


}