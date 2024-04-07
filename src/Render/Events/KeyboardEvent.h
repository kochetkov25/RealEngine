#pragma once

#include "../Event.h"

namespace Render
{
	/*обработка нажатия клавиш клавиатуры*/
	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(int key = 0):
			Event("KEY PRESSED EVENT",Event::EventType::KEY_PRESSED),
			_key(key) {}

		std::string format() const override
		{
			std::string formated = _name + ": " + char(_key);
			return formated;
		}

		inline int getKey() const { return _key; }
	private:
		int _key;
	};

	/*обработка отжатия клавиш клавиатуры*/
	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(int key = 0) :
			Event("KEY RELEASED EVENT", Event::EventType::KEY_RELEASED),
			_key(key) {}

		std::string format() const override
		{
			std::string formated = _name + ": " + char(_key);
			return formated;
		}

		inline int getKey() const { return _key; }
	private:
		int _key;
	};

}