/*
**  File        :	Texture2D.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "Texture2D.h"

namespace Render
{
	/*
	* загрузка текстуры в видеопамять для работы с ней в OpenGl.
	* В данный конструктор передается массив байт изображения,
	* для создания и отрисовки текстуры
	*/
	/*============================================================*/
	Texture2D::Texture2D(const GLuint width, const GLuint height, 
		                 const unsigned char *textureData,
					     const unsigned int channels,
		                 const GLenum filter,
						 const GLenum wrapMode)
	{
		_height = height;
		_width = width;

		// установка кол-ва каналов текстуры
		switch (channels){
		case 4:
			_mode = GL_RGBA;
			break;
		case 3:
			_mode = GL_RGB;
			break;
		default:
			_mode = GL_RGBA;
			break;
		}
		// создание id текстуры
		glGenTextures(1, &_ID);
		// в OpenGL поддерживается до 16 слотов под текстуры. Для загрузки текстуры делаем активным нулевой слот
		glActiveTexture(GL_TEXTURE0);
		// привязка текстуры к слоту (2д текстура) (явно указываем, что мы будем работать с 2д текстурой)
		glBindTexture(GL_TEXTURE_2D, _ID);
		// задание параметров текстуры
		glTexImage2D(GL_TEXTURE_2D, 0, _mode, _width, _height, 0, _mode, GL_UNSIGNED_BYTE, textureData);
		// установка параметров для фильтрации текстуры 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		// установка параметров для мипмапов
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		// генерация мипмап текстуры
		glGenerateMipmap(GL_TEXTURE_2D);
		// отвязка текстуры от слота (чтобы не создавать путаницу) 
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/*============================================================*/
	Texture2D::~Texture2D()
	{
		// освобождение ресурсов видеопамяти
		glDeleteTextures(1, &_ID);
	}

	/*============================================================*/
	// делаем текущую текстуру активной
	void Texture2D::bindTexture2D()
	{
		glBindTexture(GL_TEXTURE_2D, _ID);
	}
	
	/*============================================================*/
	// добавление текстуры из текстурного атласа
	void Texture2D::addSubTexture2D(std::string textureName, 
									const glm::vec2 &leftBottomUV, 
									const glm::vec2 &rightTopUV)
	{
		_subTextures2Dmap.emplace(textureName, subTexture2D(leftBottomUV, rightTopUV));
	}

	/*============================================================*/
	/*
	* функция возвращает текстуру из текстурного атласа. 
	* Если текстура не найдена возвращается вся 
	* текстура текстурного атласа целиком
	*/
	Texture2D::subTexture2D& Texture2D::getSubTexture2D(const std::string &textureName)
	{
		auto it = _subTextures2Dmap.find(textureName);
		if (it != _subTextures2Dmap.end()){
			return it->second;
		}
		// если текстура не найдена, возвращаем текстуру по умолчанию
		static subTexture2D defTexture2D;
		return defTexture2D;
	}
}