#include "../Input/Input.h"

namespace Core
{
	/*определение*/
	bool Input::_keys[static_cast<size_t>(GLFW_KEY_LAST)] = {};

	/*============================================================*/
	/*проверка нажатия клавиши*/
	bool Input::isKeyPressed(const int key)
	{
		return _keys[static_cast<size_t>(key)];
	}

	/*============================================================*/
	/*отжатие клавищи*/
	void Input::releaseKey(const int key)
	{
		_keys[static_cast<size_t>(key)] = false;
	}

	/*============================================================*/
	/*нажатие клавиши*/
	void Input::pressKey(const int key)
	{
		_keys[static_cast<size_t>(key)] = true;
	}

}