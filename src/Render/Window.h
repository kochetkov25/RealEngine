/*
**  File        :	Window.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Event.h"

#include <string>
#include <functional>

namespace Render
{
	class Event;
	class EventDispatcher;

	class Window
	{
	public:
		/*конструктор*/
		Window();
		/*деструктор*/
		~Window();

		/*запрещаю использование конструкторов копирования*/
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		/*обновление окна каждый кадр*/
		void update();
		/*инициализация окна*/
		bool init();

		void initEvents();

		/*получение параметров окна*/
		unsigned int getWidth() const { return _width; };
		unsigned int getHeight() const { return _height; };
		GLFWwindow* getWindow() const { return _pWindow; };

		/*установка параметров окна*/
		void setName(std::string name) { _windowName = std::move(name); };
		void setResolution(unsigned int width, unsigned int height) 
		{ _height = height; _width = width; };

		/*диспетчер событий*/
		EventDispatcher _dispatcher;
	private:
		std::string _windowName; // название окна

		GLFWwindow* _pWindow; // указатель на GL контекст окна

		unsigned int _height;
		unsigned int _width;

		bool init_GLFW();
		bool init_GLAD();

		bool _init; // инициализация окна

		/*полиморфная обертка для callBack*/
		std::function<void(Event&)> funCallBack;
		/*функция для установки callBack*/
		void setFunCallBack(const std::function<void(Event&)>& callBack)
		{
			funCallBack = callBack;
		}

		/*callBacks*/
		/*callBack для движения мыши*/
		static void mouseMovedCallBack(GLFWwindow* pWindow, double x, double y);
		/*callBack для закрытия окна*/
		static void windowClosedCallBack(GLFWwindow* pWindow);
		/*callBack для нажатия клавиш клавиатуры*/
		static void keyPressedCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
	};
}