/*
**  File        :	Texture2D.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once
#include <memory>
#include <string>
#include <map>

#include "glad\glad.h"
#include "glm\vec2.hpp"

namespace Render{
	class Texture2D
	{
	public:
		/*структура для хранения саб текстуры*/
		struct subTexture2D
		{
			glm::vec2 _leftBottomUV;
			glm::vec2 _rightTopUV;

			/*конструктор для задания вспомогательной текстуры*/
			subTexture2D(const glm::vec2 &leftbottomUV, const glm::vec2 &rightTopUV) : _leftBottomUV(leftbottomUV),
																					   _rightTopUV(rightTopUV)
			{}
			/*конструктор для задания вспомогательной текстуры по умолчанию*/
			subTexture2D() : _leftBottomUV(0.f),
				             _rightTopUV(1.f)
			{}
		};
		/*
		* загрузка текстуры в видеопамять для работы с ней в OpenGl.
		* В данный конструктор передается массив байт изображения,
		* для создания и отрисовки текстуры
		*/
		Texture2D(const GLuint width, const GLuint height, const unsigned char *textureData, 
				  const unsigned int channels = 4,
				  const GLenum filter = GL_LINEAR,
			      const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		/*делаем текстуру активной*/
		void bindTexture2D();
		/*добавление новой текстуры из текстурного атласа*/
		void addSubTexture2D(std::string textureName, const glm::vec2 &leftBottomUV, const glm::vec2 &rightTopUV);
		/*воозвращает указатель на текстуру из текстурного атласа*/
		subTexture2D& getSubTexture2D(const std::string &textureName);
		/*возвращает ширину текстуры*/
		unsigned int getWidth() { return _width; }
		/*возвращает высоту текстуры*/
		unsigned int getHeight() { return _height; }
		/*освобождение памяти под текстуру*/
		~Texture2D();

	private:
		GLuint _ID;
		GLenum _mode;
		unsigned int _width;
		unsigned int _height;
		/*мап для хранения текстур из текстурного атласа*/
		std::map<std::string, subTexture2D> _subTextures2Dmap;
	};
}