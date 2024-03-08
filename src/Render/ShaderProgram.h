/*
**  File        :	ShaderProgram.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once
#include <glad\glad.h>
#include <glm\mat4x4.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>

namespace Render{
	class ShaderProgram{
	public:
		// ����������� ��������� �� ���� ��� �� ������ � ����� �������� � �����������
		// ��������� ���������.
		ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
		~ShaderProgram();
		// ���������� ��������� ���������� ��������� ���������
		bool isCompiled() const { return _isCompiled; }
		// ������������� ��������� OpenGL ������������� ��������� ���������
		void use() const;
		// ������������� ������� �������� ��� ��������.
		void setTexUniform(const std::string &textureName, const GLint value);
		void setMatrix4Uniform(const std::string &matrixName, const glm::mat4 &matrix);
		// ���������� ������������� ��������� ���������
		GLuint getShaderProgramID() { return _ID; }

	private:
		// ������� ������
		bool createShader(const std::string &shaderSource, const GLenum shaderType, GLuint &shaderID);
		// ������ ���������� ��������� ���������
		bool _isCompiled = false;
		// ������������� ��������� ���������
		GLuint _ID = 0;
	};
}