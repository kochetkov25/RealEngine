#pragma once

#include <GLFW/glfw3.h>

namespace Core
{
	class Input
	{
	public:
		/*проверка нажатия клавиши*/
		static bool isKeyPressed(const int key);

		/*отжатие клавищи*/
		static void releaseKey(const int key);

		/*нажатие клавиши*/
		static void pressKey(const int key);
	private:
		/*массив всех возможных клавиш клавиатуры*/
		static bool _keys[static_cast<size_t>(GLFW_KEY_LAST)];
	};
}