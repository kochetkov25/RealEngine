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
#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"

#include "../Input/Input.h"

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

		setFunCallBack([this](Event& e)
			{
				this->_dispatcher.dispatch(e);
			});
	}

	/*============================================================*/
	/*инициализация контекста GLFW*/
	bool Render::Window::init_GLFW()
	{
		if (!glfwInit())
		{
			std::cout << "Error load GLFW!" << std::endl;
			return false;
		}
		return true;
	}

	/*============================================================*/
	/*инициализация контекста GLAD*/
	bool Render::Window::init_GLAD()
	{
		if (!gladLoadGL()) {
			std::cout << "Error load GLAD!" << std::endl;
			return false;
		}
		else {
			std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
			std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;
		}
		return true;
	}

	/*============================================================*/
	/*регистрация callBack_ов для событий GL*/
	void Render::Window::initEvents()
	{
		_dispatcher.addEventListner<MouseMovedEvent>(
			[](MouseMovedEvent& e)
			{
				Core::Input::setMousePosition(e.getPosition());
			}
		);

		_dispatcher.addEventListner<MouseButtonPressed>(
			[](MouseButtonPressed& e)
			{
				std::cout << e.format() << std::endl;
				Core::Input::pressMouseBtn(e.getButton());
			}
		);

		_dispatcher.addEventListner<MouseButtonReleased>(
			[](MouseButtonReleased& e)
			{
				std::cout << e.format() << std::endl;
				Core::Input::releaseMouseBtn(e.getButton());
			}
		);

		_dispatcher.addEventListner<WindowClosedEvent>(
			[](WindowClosedEvent& e)
			{
				std::cout << e.format() << std::endl;
			}
		);

		_dispatcher.addEventListner<KeyPressedEvent>(
			[](KeyPressedEvent& e)
			{
				std::cout << e.format() << std::endl;
				Core::Input::pressKey(e.getKey());
			}
		);

		_dispatcher.addEventListner<KeyReleasedEvent>(
			[](KeyReleasedEvent& e)
			{
				std::cout << e.format() << std::endl;
				Core::Input::releaseKey(e.getKey());
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
		if (!init_GLFW())
			return false;
		/*запрещаю масщтабирование окна*/
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

		/*регистрация callBack_ов*/
		initEvents();
		/*установка callBack функций*/
		glfwSetWindowUserPointer(_pWindow, this);
		/*callBack движение мыши*/
		glfwSetCursorPosCallback(_pWindow, mouseMovedCallBack);
		/*callback для закрытия окна*/
		glfwSetWindowCloseCallback(_pWindow, windowClosedCallBack);
		/*callback для нажатия клавиш*/
		glfwSetKeyCallback(_pWindow, keyCallBack);
		/*callback для нажатия кнопок мыши*/
		glfwSetMouseButtonCallback(_pWindow, mouseButtonCallBack);

		/*инициализация GLAD*/
		if (!init_GLAD())
			return false;

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

	/*============================================================*/
	/*callBack при движении мышки*/
	void Render::Window::windowClosedCallBack(GLFWwindow* pWindow)
	{
		/*получаем ссылку на окно с пользовательскими данными*/
		auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
		WindowClosedEvent event;
		handle.funCallBack(event);
	}

	/*============================================================*/
	/*callBack при нажатии клавиш*/
	void Render::Window::keyCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
	{
		/*получаем ссылку на окно с пользовательскими данными*/
		auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent eventPressed(key);
			handle.funCallBack(eventPressed);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent eventReleased(key);
			handle.funCallBack(eventReleased);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	/*============================================================*/
	/*callBack для нажатия кнопок мыши*/
	void Window::mouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods)
	{
		auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
		if (action == GLFW_PRESS)
		{
			MouseButtonPressed eventMousePressed(button);
			handle.funCallBack(eventMousePressed);
		}
		else
		{
			MouseButtonReleased eventMouseReleased(button);
			handle.funCallBack(eventMouseReleased);
		}
	}
}