#pragma once

#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

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

		static void pressMouseBtn(const int key);

		static void releaseMouseBtn(const int key);

		static bool isMouseBtnPressed(const int key);

		static void setMousePosition(glm::vec2& vec);

		static glm::vec2 getMousePosition();
	private:
		/*массив всех возможных клавиш клавиатуры*/
		static bool _keys[static_cast<size_t>(GLFW_KEY_LAST)];

		static bool _mouseBtns[static_cast<size_t>(GLFW_MOUSE_BUTTON_LAST)];

		static glm::vec2 _mousePos;
	};
}