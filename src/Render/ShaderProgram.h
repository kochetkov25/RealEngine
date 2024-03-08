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
		// конструктор принимает на вход две си строки с кодом шейдеров и компилирует
		// шейдерную программу.
		ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
		~ShaderProgram();
		// возвращает результат компил€ции шейдерной программы
		bool isCompiled() const { return _isCompiled; }
		// устанавливает контексту OpenGL использование шейдерной программы
		void use() const;
		// устанавливаем униформ значение дл€ текстуры.
		void setTexUniform(const std::string &textureName, const GLint value);
		void setMatrix4Uniform(const std::string &matrixName, const glm::mat4 &matrix);
		// возвращает идентификатор шейдерной программы
		GLuint getShaderProgramID() { return _ID; }

	private:
		// создает шейдер
		bool createShader(const std::string &shaderSource, const GLenum shaderType, GLuint &shaderID);
		// статус компил€ции шейдерной программы
		bool _isCompiled = false;
		// идентификатор шейдерной программы
		GLuint _ID = 0;
	};
}