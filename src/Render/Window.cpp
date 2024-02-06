#include "Window.h"

namespace Render
{
	/*============================================================*/
	/*� ������������ ������������ ��������� �� ���������*/
	Render::Window::Window()
	{
		_init = false;
		_pWindow = nullptr;
		_width = 1024;
		_height = 768;
		_windowName = "mainWindow";
	}

	/*============================================================*/
	/*������������� ����*/
	static bool GLFWinit = false;
	bool Render::Window::init()
	{
		/*�������� �� �������������*/
		if (_init)
			return true;

		/*������������� GLFW*/
		if (!GLFWinit)
		{
			if (!glfwInit())
			{
				std::cout << "Error load GLFW!" << std::endl;
				return false;
			}
			GLFWinit = true;
		}

		/*������������� ����*/
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

		/*��������� ������� ���� ��� �������� ���������*/
		glfwMakeContextCurrent(_pWindow);

		/*������������� GLAD*/
		if (!gladLoadGL()) {
			std::cout << "Error load GLAD!" << std::endl;
			return false;
		}
		else {
			std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
			std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;
		}

		/*����������� callback ��� �������� ����*/
		glfwSetWindowCloseCallback(_pWindow, [](GLFWwindow* pWindow) 
			                                   {
				                                  glfwSetWindowShouldClose(pWindow, GL_TRUE);
			                                   });

		/*�������� �������������*/
		std::cout << "Window Inited!" << std::endl;
		_init = true;
		return true;
	}

	/*============================================================*/
	/*� ����������� ���������� ������� OpenGL*/
	Render::Window::~Window()
	{
		glfwDestroyWindow(_pWindow);
		glfwTerminate();
	}

	/*============================================================*/
	/*���������� ����*/
	void Render::Window::update()
	{
		glClearColor(0.6f, 0.69f, 0.929f, 1.f); // ������� ��� ���� ������
		glfwSwapBuffers(_pWindow); // ����� �������� � ������ �����
		glfwPollEvents(); // ��������� ��������� �������
	}
}