#include "../Input/Input.h"

namespace Core
{
	/*�����������*/
	bool Input::_keys[static_cast<size_t>(GLFW_KEY_LAST)] = {};

	/*============================================================*/
	/*�������� ������� �������*/
	bool Input::isKeyPressed(const int key)
	{
		return _keys[static_cast<size_t>(key)];
	}

	/*============================================================*/
	/*������� �������*/
	void Input::releaseKey(const int key)
	{
		_keys[static_cast<size_t>(key)] = false;
	}

	/*============================================================*/
	/*������� �������*/
	void Input::pressKey(const int key)
	{
		_keys[static_cast<size_t>(key)] = true;
	}

}