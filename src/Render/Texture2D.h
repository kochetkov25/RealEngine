#pragma once
#include <string>
#include "glad\glad.h"
#include "glm\vec2.hpp"
#include <memory>
#include <string>
#include <map>

namespace Render{
	class Texture2D
	{
	public:
		// структура для хранения саб текстуры
		// хранит координаты нижнего левого угла и верхнего правого угла
		struct subTexture2D{
			glm::vec2 _leftBottomUV;
			glm::vec2 _rightTopUV;

			// конструктор для задания вспомогательной текстуры
			subTexture2D(const glm::vec2 &leftbottomUV, const glm::vec2 &rightTopUV)
				: _leftBottomUV(leftbottomUV),
				_rightTopUV(rightTopUV)
			{}
			// конструктор для задания вспомогательной текстуры по умолчанию (одна большая текстура)
			subTexture2D()
				: _leftBottomUV(0.f),
				_rightTopUV(1.f)
			{}
		};
		// загрузка текстуры в видеопамять для работы с ней в OpenGl.
		// !!! в данный конструктор передается массив байтов, который был загружен в программу в ресурс менеджере,
		// с помощью данного конструктора задаются все необходимые параметры контекста OpenGL для работы с текстурой,
		// а также массив байтов текстуры загружается в видеопамять
		Texture2D(const GLuint width, const GLuint height, const unsigned char *textureData, 
			const unsigned int channels = 4,
			const GLenum filter=GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		// делаем текстуру активной
		void bindTexture2D();
		// добавление новой текстуры из текстурного атласа
		void addSubTexture2D(std::string textureName, const glm::vec2 &leftBottomUV, const glm::vec2 &rightTopUV);
		// воозвращает указатель на текстуру из текстурного атласа
		subTexture2D& getSubTexture2D(const std::string &textureName);
		// возвращает ширину текстуры
		unsigned int getWidth() { return _width; }
		// возвращает высоту текстуры
		unsigned int getHeight() { return _height; }
		// освобождение памяти под текстуру
		~Texture2D();

	private:
		GLuint _ID;
		GLenum _mode;
		unsigned int _width;
		unsigned int _height;
		// мап для хранения текстур из текстурного атласа
		std::map<std::string, subTexture2D> _subTextures2Dmap;
	};
}