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
	* �������� �������� � ����������� ��� ������ � ��� � OpenGl.
	* � ������ ����������� ���������� ������ ���� �����������,
	* ��� �������� � ��������� ��������
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

		// ��������� ���-�� ������� ��������
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
		// �������� id ��������
		glGenTextures(1, &_ID);
		// � OpenGL �������������� �� 16 ������ ��� ��������. ��� �������� �������� ������ �������� ������� ����
		glActiveTexture(GL_TEXTURE0);
		// �������� �������� � ����� (2� ��������) (���� ���������, ��� �� ����� �������� � 2� ���������)
		glBindTexture(GL_TEXTURE_2D, _ID);
		// ������� ���������� ��������
		glTexImage2D(GL_TEXTURE_2D, 0, _mode, _width, _height, 0, _mode, GL_UNSIGNED_BYTE, textureData);
		// ��������� ���������� ��� ���������� �������� 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		// ��������� ���������� ��� ��������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		// ��������� ������ ��������
		glGenerateMipmap(GL_TEXTURE_2D);
		// ������� �������� �� ����� (����� �� ��������� ��������) 
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/*============================================================*/
	Texture2D::~Texture2D()
	{
		// ������������ �������� �����������
		glDeleteTextures(1, &_ID);
	}

	/*============================================================*/
	// ������ ������� �������� ��������
	void Texture2D::bindTexture2D()
	{
		glBindTexture(GL_TEXTURE_2D, _ID);
	}
	
	/*============================================================*/
	// ���������� �������� �� ����������� ������
	void Texture2D::addSubTexture2D(std::string textureName, 
									const glm::vec2 &leftBottomUV, 
									const glm::vec2 &rightTopUV)
	{
		_subTextures2Dmap.emplace(textureName, subTexture2D(leftBottomUV, rightTopUV));
	}

	/*============================================================*/
	/*
	* ������� ���������� �������� �� ����������� ������. 
	* ���� �������� �� ������� ������������ ��� 
	* �������� ����������� ������ �������
	*/
	Texture2D::subTexture2D& Texture2D::getSubTexture2D(const std::string &textureName)
	{
		auto it = _subTextures2Dmap.find(textureName);
		if (it != _subTextures2Dmap.end()){
			return it->second;
		}
		// ���� �������� �� �������, ���������� �������� �� ���������
		static subTexture2D defTexture2D;
		return defTexture2D;
	}
}