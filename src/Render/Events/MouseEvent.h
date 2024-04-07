#pragma once

#include "../Event.h"

#include "glm/vec2.hpp"

namespace Render
{
	/*обработка перемещения курсора*/
	class MouseMovedEvent : public Event
	{
	public:
		/*конструктор*/
		MouseMovedEvent(int x = 0, int y = 0) :
			Event("MOUSE MOVED EVENT", EventType::MOUSE_MOVED),
			_x(x),
			_y(y){}
		/*вывод информации о событии*/
		std::string format() const override
		{
			std::string formated = _name + ": " + "X = " +std::to_string(_x) + ", Y = " + std::to_string(_y);
			return formated;
		}

		glm::vec2 getPosition() { return glm::vec2(_x, _y); }
	private:
		int _x;
		int _y;
	};

	/*обработка нажатия кнопок мыши*/
	class MouseButtonPressed : public Event
	{
	public:
		MouseButtonPressed(int button = 0)
			:
			Event("MOUSE BUTTON PRESSED", EventType::MOUSE_BUTTON_PRESSED),
			_button(button) {}

		std::string format() const override
		{
			std::string formated = _name + ": " + std::to_string(_button);
			return formated;
		}

		int getButton() { return _button; }
	private:
		int _button = 0;
	};

	/*обработка отжатия кнопок мыши*/
	class MouseButtonReleased : public Event
	{
	public:
		MouseButtonReleased(int button = 0)
			:
			Event("MOUSE BUTTON RELEASED", EventType::MOUSE_BUTTON_RELEASED),
			_button(button) {}

		std::string format() const override
		{
			std::string formated = _name + ": " + std::to_string(_button);
			return formated;
		}

		int getButton() { return _button; }
	private:
		int _button;
	};
}