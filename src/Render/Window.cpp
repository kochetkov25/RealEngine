/*
**  File        :	Window.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "Window.h"

#include "../Modules/GUIModule.h"

#include "Events/MouseEvent.h"

#include <iostream>

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

		/*устанавливаем дефолтный callBack*/
		/*setFunCallBack([](Event& e)
			{
				std::cout << e.format() << std::endl;
			});*/

		setFunCallBack([this](Event& e)
			{
				this->_dispatcher.dispatch(e);
			});
	}

	void Render::Window::initEvents()
	{
		_dispatcher.addEventListner<MouseMovedEvent>(
			[](MouseMovedEvent& e)
			{
				std::cout << e.format() << std::endl;
			}
		);
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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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

		initEvents();
		/*установка callBack функций*/
		glfwSetWindowUserPointer(_pWindow, this);
		/*callBack движение мыши*/
		glfwSetCursorPosCallback(_pWindow, mouseMovedCallBack);

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
		Modules::GUIModule::onWindowClose();
		glfwDestroyWindow(_pWindow);
		glfwTerminate();
	}

	/*============================================================*/
	/*обновление окна*/
	void Render::Window::update()
	{
		glfwSwapBuffers(_pWindow); // меняю передний и задний буфер
		glfwPollEvents(); // произвожу обработку событий
	}

	/*============================================================*/
	/*callBack при движении мышки*/
	void Window::mouseMovedCallBack(GLFWwindow* pWindow, double x, double y)
	{
		/*получаем ссылку на окно с пользовательскими данными*/
		auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
		/*создаем event для движения мыши*/
		MouseMovedEvent event(x, y);
		/*выполяем callBack для движения мыши*/
		handle.funCallBack(event);
	}
}