/*
**  File        :	Texture2D.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once
#include <string>
#include <map>

#include "glad/glad.h"
#include "glm/vec2.hpp"

namespace Render
{


	class Texture2D
	{
	public:
		/*��������� ��� �������� ��� ��������*/
		struct subTexture2D
		{
			glm::vec2 _leftBottomUV;
			glm::vec2 _rightTopUV;

			/*����������� ��� ������� ��������������� ��������*/
			subTexture2D(const glm::vec2 &leftbottomUV, const glm::vec2 &rightTopUV) : _leftBottomUV(leftbottomUV),
																					   _rightTopUV(rightTopUV)
			{}
			/*����������� ��� ������� ��������������� �������� �� ���������*/
			subTexture2D() : _leftBottomUV(0.f),
				             _rightTopUV(1.f)
			{}
		};

		/*
		* �������� �������� � ����������� ��� ������ � ��� � OpenGl.
		* � ������ ����������� ���������� ������ ���� �����������,
		* ��� �������� � ��������� ��������
		*/
		Texture2D(
					  const GLuint width, 
					  const GLuint height, 
					  const unsigned char *textureData, 
					  const unsigned int channels = 4,
					  const GLenum filter = GL_LINEAR,
					  const GLenum wrapMode = GL_CLAMP_TO_EDGE
			     );

		Texture2D(
					  const GLuint width,
					  const GLuint height,
					  const float* textureData,
					  const unsigned int channels = 4,
					  const GLenum filter = GL_LINEAR,
					  const GLenum wrapMode = GL_CLAMP_TO_EDGE
		        );
		/*������ �������� ��������*/
		void bindTexture2D(const unsigned short texBlock = 0);

		/*���������� ����� �������� �� ����������� ������*/
		void addSubTexture2D(std::string textureName, const glm::vec2 &leftBottomUV, const glm::vec2 &rightTopUV);

		/*����������� ��������� �� �������� �� ����������� ������*/
		subTexture2D& getSubTexture2D(const std::string &textureName);

		/*���������� ������ ��������*/
		unsigned int getWidth() { return _width; }

		/*���������� ������ ��������*/
		unsigned int getHeight() { return _height; }

		/*������������ ������ ��� ��������*/
		~Texture2D();

	private:
		GLuint _ID;
		GLenum _mode;
		unsigned int _width;
		unsigned int _height;
		/*map ��� �������� ������� �� ����������� ������*/
		std::map<std::string, subTexture2D> _subTextures2Dmap;
	};


}