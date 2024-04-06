#pragma once

#include "../Event.h"

namespace Render
{
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() :
			Event("Window Closed Event",Event::EventType::WINDOW_CLOSED) {}

		/*вывод информации о событии*/
		std::string format() const override
		{
			std::string formated = _name + "!";
			return formated;
		}
	private:
	};
}