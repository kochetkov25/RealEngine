#include "Window.h"

namespace Render
{
	/*============================================================*/
	/*в конструкторе устанавливаю параметры по умолчанию*/
	Render::Window::Window()
	{
		_init = false;
		_pWindow = nullptr;
		_width = 1024;
		_height = 768;
		_windowName = "mainWindow";
	}

	/*============================================================*/
	/*инициализация окна*/
	static bool GLFWinit = false;
	bool Render::Window::init()
	{
		/*проверка на инициализацию*/
		if (_init)
			return true;

		/*инициализация GLFW*/
		if (!GLFWinit)
		{
			if (!glfwInit())
			{
				std::cout << "Error load GLFW!" << std::endl;
				return false;
			}
			GLFWinit = true;
		}

		/*инициализация окна*/
		if (!_pWindow)
		{
			_pWindow = glfwCreateWindow(_width, _height, _windowName.c_str(), NULL, NULL);
			if (!_pWindow)
			{
				std::cout << "Error create window: " << _windowName << std::endl;
				glfwTerminate();
				return false;
			}
		}

		/*установка данного окна как контекст отрисовки*/
		glfwMakeContextCurrent(_pWindow);

		/*инициализация GLAD*/
		if (!gladLoadGL()) {
			std::cout << "Error load GLAD!" << std::endl;
			return false;
		}
		else {
			std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
			std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;
		}

		/*регестрирую callback для закрытия окна*/
		glfwSetWindowCloseCallback(_pWindow, [](GLFWwindow* pWindow) 
			                                   {
				                                  glfwSetWindowShouldClose(pWindow, GL_TRUE);
			                                   });

		/*успешная инициализация*/
		std::cout << "Window Inited!" << std::endl;
		_init = true;
		return true;
	}

	/*============================================================*/
	/*в деструкторе освобождаю ресурсы OpenGL*/
	Render::Window::~Window()
	{
		glfwDestroyWindow(_pWindow);
		glfwTerminate();
	}

	/*============================================================*/
	/*обновление окна*/
	void Render::Window::update()
	{
		glClearColor(0.6f, 0.69f, 0.929f, 1.f); // заливаю все окно цветом
		glfwSwapBuffers(_pWindow); // меняю передний и задний буфер
		glfwPollEvents(); // произвожу обработку событий
	}
}