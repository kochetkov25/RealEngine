#include "ShaderProgram.h"
#include <glm\gtc\type_ptr.hpp>

namespace Render{
	// конструктор класса
	ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
	{
		// создаем вершинный шейдер
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)){
			std::cerr << "Error compile VERTEX SHADER. (source: ShaderProgram)" << std::endl;
			return;
		}

		// создаем фрагментный шейдер
		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)){
			std::cerr << "Error compile FRAGMENT SHADER. (source: ShaderProgram)" << std::endl;
			// фрагментный шейдер не скомпилировался удаляем также и вершинный шейдер
			glDeleteShader(vertexShaderID);
			return;
		}

		// соединяем шейдеры в шейдерную программу
		_ID = glCreateProgram();
		glAttachShader(_ID, vertexShaderID);
		glAttachShader(_ID, fragmentShaderID);
		glLinkProgram(_ID);

		// проверка ошибок линковки шейдерной программы
		GLint isSuccessLink;
		glGetProgramiv(_ID,GL_LINK_STATUS,&isSuccessLink);
		if (!isSuccessLink){
			GLchar infoLog[1024];
			glGetProgramInfoLog(_ID, 1024, nullptr, infoLog);
			std::cerr << "Error link SHADER PROGRAM (source: ShaderProgram): \n" << infoLog << std::endl;
		}
		else
		{
			// при успешной линковке устанавливаем флаг true
			_isCompiled = true;
		}

		// освобождение ресурсов под шейдеры
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram::~ShaderProgram(){
		// в деструкторе удаляем ресурсы шейдерной программы
		glDeleteProgram(_ID);
	}
	
	// создание нужного шейдера
	bool ShaderProgram::createShader(const std::string &shaderSource, const GLenum shaderType, GLuint &shaderID)
	{
		// создание шейдера по переданному имени шейдера
		shaderID = glCreateShader(shaderType);
		const char* code = shaderSource.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		// проверка ошибок компиляции шейдера
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

	// задание использования шейдерной программы
	void ShaderProgram::use()const
	{
		glUseProgram(_ID);
	}
	// устанавливаем униформ значение для текстуры. value это слот в который записана текстура
	void ShaderProgram::setTexUniform(const std::string &textureName, const GLint value)
	{
		glUniform1i(glGetUniformLocation(_ID, textureName.c_str()), value);
	}

	void ShaderProgram::setMatrix4Uniform(const std::string &matrixName, const glm::mat4 &matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(_ID, matrixName.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}