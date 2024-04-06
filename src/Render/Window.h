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
		/*�����������*/
		Window();
		/*����������*/
		~Window();

		/*�������� ������������� ������������� �����������*/
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		/*���������� ���� ������ ����*/
		void update();
		/*������������� ����*/
		bool init();

		void initEvents();

		/*��������� ���������� ����*/
		unsigned int getWidth() const { return _width; };
		unsigned int getHeight() const { return _height; };
		GLFWwindow* getWindow() const { return _pWindow; };

		/*��������� ���������� ����*/
		void setName(std::string name) { _windowName = std::move(name); };
		void setResolution(unsigned int width, unsigned int height) 
		{ _height = height; _width = width; };

		/*��������� �������*/
		EventDispatcher _dispatcher;
	private:
		std::string _windowName; // �������� ����

		GLFWwindow* _pWindow; // ��������� �� GL �������� ����

		unsigned int _height;
		unsigned int _width;

		bool init_GLFW();
		bool init_GLAD();

		bool _init; // ������������� ����

		/*����������� ������� ��� callBack*/
		std::function<void(Event&)> funCallBack;
		/*������� ��� ��������� callBack*/
		void setFunCallBack(const std::function<void(Event&)>& callBack)
		{
			funCallBack = callBack;
		}

		/*callBacks*/
		/*callBack ��� �������� ����*/
		static void mouseMovedCallBack(GLFWwindow* pWindow, double x, double y);
		/*callBack ��� �������� ����*/
		static void windowClosedCallBack(GLFWwindow* pWindow);
		/*callBack ��� ������� ������ ����������*/
		static void keyPressedCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
	};
}