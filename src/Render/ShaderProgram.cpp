/*
**  File        :	ShaderProgram.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "ShaderProgram.h"

#include <iostream>

#include <glm\gtc\type_ptr.hpp>

namespace Render
{


	/*============================================================*/
	/*конструктор. компилирует шейдерную программу*/
	ShaderProgram::ShaderProgram(
									 const std::string &vertexShader, 
									 const std::string &fragmentShader
								)
	{
		/*создаем вершинный шейдер*/
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)){
			std::cerr << "Error compile VERTEX SHADER. (source: " << __FUNCTION__<< ")" << std::endl;
			return;
		}

		/*создаем фрагментный шейдер*/
		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)){
			std::cerr << "Error compile FRAGMENT SHADER. (source: " << __FUNCTION__ << ")" << std::endl;
			// фрагментный шейдер не скомпилировался удаляем также и вершинный шейдер
			glDeleteShader(vertexShaderID);
			return;
		}

		/*соединяем шейдеры в шейдерную программу*/
		_ID = glCreateProgram();
		glAttachShader(_ID, vertexShaderID);
		glAttachShader(_ID, fragmentShaderID);
		glLinkProgram(_ID);

		/*проверка ошибок линковки шейдерной программы*/
		GLint isSuccessLink;
		glGetProgramiv(_ID,GL_LINK_STATUS,&isSuccessLink);
		if (!isSuccessLink){
			GLchar infoLog[1024];
			glGetProgramInfoLog(_ID, 1024, nullptr, infoLog);
			std::cerr << "Error link SHADER PROGRAM (source: " << __FUNCTION__ << ") \n" << infoLog << std::endl;
		}
		else
		{
			/*компиляция прошла успешно*/
			_isCompiled = true;
		}

		/*освобождение ресурсов под шейдеры*/
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	/*============================================================*/
	/*в деструкторе удаляем ресурсы шейдерной программы*/
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(_ID);
	}
	
	/*============================================================*/
	/*создание шейдера*/
	bool ShaderProgram::createShader(
										 const std::string &shaderSource, 
										 const GLenum shaderType, 
										 GLuint &shaderID
									)
	{
		/*создание шейдера по переданному имени шейдера*/
		shaderID = glCreateShader(shaderType);
		const char* code = shaderSource.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		/*проверка ошибок компиляции шейдера*/
		GLint isSuccessCompile;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isSuccessCompile);
		if (!isSuccessCompile){
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::cerr << "Error compile shader (source: " << __FUNCTION__ << ") \n"<< infoLog << std::endl;
			return false;
		}
		return true;
	}

	/*============================================================*/
	/*устанавливает контексту OpenGL использование 
	  данной шейдерной программы*/
	void ShaderProgram::use()const
	{
		glUseProgram(_ID);
	}

	/*============================================================*/
	/*установить uniform значение для текстуры*/
	void ShaderProgram::setTexUniform(
										const std::string &textureName, 
										const GLint value
									 )
	{
		glUniform1i(glGetUniformLocation(_ID, textureName.c_str()), value);
	}

	/*============================================================*/
	/*утсановить значение uniform mat4*/
	void ShaderProgram::setMatrix4Uniform(
											  const std::string &matrixName, 
											  const glm::mat4 &matrix
										 )
	{
		glUniformMatrix4fv(glGetUniformLocation(_ID, matrixName.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	/*============================================================*/
	/*утсановить значение uniform vec3*/
	void Render::ShaderProgram::serVec3Uniform(
											  	const std::string& vecName,
											  	const glm::vec3& vec3
											  )
	{
		glUniform3f(glGetUniformLocation(_ID, vecName.c_str()), vec3.x, vec3.y, vec3.z);
	}

	/*============================================================*/
	/*утсановить значение float uniform*/
	void Render::ShaderProgram::setFloatUniform(const std::string& name, const float value)
	{
		glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
	}

	/*============================================================*/
	/**/
	void Render::ShaderProgram::setArrayUniform(const std::string& name, const size_t size, const float* arr)
	{
		glUniform1fv(glGetUniformLocation(_ID, name.c_str()), static_cast<float>(size), arr);
	}
}