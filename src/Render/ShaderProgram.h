/*
**  File        :	ShaderProgram.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once
#include <glad/glad.h>
#include <string>

#include <glm/mat4x4.hpp>

namespace Render
{


	class ShaderProgram{
	public:
		/*�����������. ����������� ��������� ���������*/
		ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);

		/*����������*/
		~ShaderProgram();

		/*��������� ���������� ��������� ���������*/
		bool isCompiled() const { return _isCompiled; }

		/*������������� ��������� OpenGL ������������� ��������� ���������*/
		void use() const;

		/*���������� uniform �������� ��� ��������*/
		void setTexUniform(const std::string &textureName, const GLint value);

		/*���������� �������� uniform mat4*/
		void setMatrix4Uniform(const std::string &vecName, const glm::mat4 &matrix);

		/*���������� �������� uniform vec3*/
		void serVec3Uniform(const std::string& matrixName, const glm::vec3& vec3);

		/*���������� �������� float uniform*/
		void setFloatUniform(const std::string& name, const float value);

		/**/
		void setArrayUniform(const std::string& name, const size_t size, const float* arr);

		/*������������� ��������� ���������*/
		GLuint getShaderProgramID() { return _ID; }

	private:
		/*�������� �������*/
		bool createShader(const std::string &shaderSource, const GLenum shaderType, GLuint &shaderID);

		/*������ ���������� ��������� ���������*/
		bool _isCompiled = false;

		/* ������������� ��������� ���������*/
		GLuint _ID = 0;
	};


}