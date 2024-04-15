/*
**  File        :	ShaderProgram.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once
#include <glad\glad.h>
#include <string>

#include <glm\mat4x4.hpp>

namespace Render
{


	class ShaderProgram{
	public:
		/*конструктор. компилирует шейдерную программу*/
		ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);

		/*деструктор*/
		~ShaderProgram();

		/*результат компил€ции шейдерной программы*/
		bool isCompiled() const { return _isCompiled; }

		/*устанавливает контексту OpenGL использование шейдерной программы*/
		void use() const;

		/*установить uniform значение дл€ текстуры*/
		void setTexUniform(const std::string &textureName, const GLint value);

		/*утсановить значение uniform mat4*/
		void setMatrix4Uniform(const std::string &vecName, const glm::mat4 &matrix);

		/*утсановить значение uniform vec3*/
		void serVec3Uniform(const std::string& matrixName, const glm::vec3& vec3);

		/*утсановить значение float uniform*/
		void setFloatUniform(const std::string& name, const float value);

		/**/
		void setArrayUniform(const std::string& name, const size_t size, const float* arr);

		/*идентификатор шейдерной программы*/
		GLuint getShaderProgramID() { return _ID; }

	private:
		/*создание шейдера*/
		bool createShader(const std::string &shaderSource, const GLenum shaderType, GLuint &shaderID);

		/*статус компил€ции шейдерной программы*/
		bool _isCompiled = false;

		/* идентификатор шейдерной программы*/
		GLuint _ID = 0;
	};


}