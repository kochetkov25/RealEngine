/*
**  File        :	ShaderProgram.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "ShaderProgram.h"

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
			std::cerr << "Error compile VERTEX SHADER. (source: ShaderProgram)" << std::endl;
			return;
		}

		/*создаем фрагментный шейдер*/
		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)){
			std::cerr << "Error compile FRAGMENT SHADER. (source: ShaderProgram)" << std::endl;
			// фрагментный шейдер не скомпилировалс€ удал€ем также и вершинный шейдер
			glDeleteShader(vertexShaderID);
			return;
		}

		/*соедин€ем шейдеры в шейдерную программу*/
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
			std::cerr << "Error link SHADER PROGRAM (source: ShaderProgram): \n" << infoLog << std::endl;
		}
		else
		{
			/*компил€ци€ прошла успешно*/
			_isCompiled = true;
		}

		/*освобождение ресурсов под шейдеры*/
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	/*============================================================*/
	/*в деструкторе удал€ем ресурсы шейдерной программы*/
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

		/*проверка ошибок компил€ции шейдера*/
		GLint isSuccessCompile;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isSuccessCompile);
		if (!isSuccessCompile){
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::cerr << "Error compile shader (source: ShaderProgram): \n" << infoLog << std::endl;
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
	/*установить uniform значение дл€ текстуры*/
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


}