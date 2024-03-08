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

		/*��������� ���������� ����*/
		unsigned int getWidth() const { return _width; };
		unsigned int getHeight() const { return _height; };
		GLFWwindow* getWindow() const { return _pWindow; };

		/*��������� ���������� ����*/
		void setName(std::string name) { _windowName = std::move(name); };
		void setResolution(unsigned int width, unsigned int height) 
		{ _height = height; _width = width; };
	private:
		std::string _windowName; // �������� ����

		GLFWwindow* _pWindow; // ��������� �� �������� ����

		unsigned int _height;
		unsigned int _width;

		bool _init; // ������������� ����
	};
}