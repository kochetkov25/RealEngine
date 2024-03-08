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


#include <string>
#include <iostream>

namespace Render
{
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

		/*получение параметров окна*/
		unsigned int getWidth() const { return _width; };
		unsigned int getHeight() const { return _height; };
		GLFWwindow* getWindow() const { return _pWindow; };

		/*установка параметров окна*/
		void setName(std::string name) { _windowName = std::move(name); };
		void setResolution(unsigned int width, unsigned int height) 
		{ _height = height; _width = width; };
	private:
		std::string _windowName; // название окна

		GLFWwindow* _pWindow; // указатель на контекст окна

		unsigned int _height;
		unsigned int _width;

		bool _init; // инициализация окна
	};
}