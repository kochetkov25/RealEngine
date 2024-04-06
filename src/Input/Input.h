#pragma once

#include <GLFW/glfw3.h>

namespace Core
{
	class Input
	{
	public:
		/*�������� ������� �������*/
		static bool isKeyPressed(const int key);

		/*������� �������*/
		static void releaseKey(const int key);

		/*������� �������*/
		static void pressKey(const int key);
	private:
		/*������ ���� ��������� ������ ����������*/
		static bool _keys[static_cast<size_t>(GLFW_KEY_LAST)];
	};
}