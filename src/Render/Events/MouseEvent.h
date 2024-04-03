#pragma once

#include "../Event.h"

namespace Render
{
	class MouseMovedEvent : public Event
	{
	public:
		/*�����������*/
		MouseMovedEvent(int x, int y) :
			Event("MOUSE MOVED EVENT",EventType::MOUSE_MOVED),
			_x(x),
			_y(y){}
		/*����� ���������� � �������*/
		std::string format() const override
		{
			std::string formated = _name + ": " + "X = " +std::to_string(_x) + ", Y = " + std::to_string(_y);
			return formated;
		}
	private:
		int _x;
		int _y;
	};
}