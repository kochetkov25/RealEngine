#pragma once
#include <string>

namespace Render
{

	class Event
	{
	public:
		/*типы ивентов*/
		enum class EventType
		{
			MOUSE_MOVED
		};

		virtual ~Event() = default;

		/*имя события*/
		inline std::string getName() const { return _name; }

		/*тип события*/
		inline EventType getType() const { return _type; }

		/*чисто виртуальная функция для вывода информации о событии*/
		virtual std::string format() const = 0;
	protected:
		EventType _type;
		std::string _name;

		Event(const std::string& name, const EventType type)
			:
			_type(type),
			_name(name) {}
	};

}