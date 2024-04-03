#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>

namespace Render
{
	/*базовый класс для создания событий*/
	class Event
	{
	public:
		/*типы ивентов*/
		enum class EventType
		{
			MOUSE_MOVED = 1,

			COUNT
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

	/*диспетчер событий*/
	class EventDispatcher
	{
	public:
		template<typename _typeEvent>
		void addEventListner(std::function<void(_typeEvent&)> callBack)
		{
			auto base = [func = std::move(callBack)](Event& e)
			{
				func(static_cast<_typeEvent&>(e));
			};
			_vecEventCallBacks.emplace_back(base);
		}

		void dispatch(Event& event)
		{
			size_t type = static_cast<size_t>(event.getType());
			if (type > _vecEventCallBacks.size())
			{
				std::cerr << "Event not set!" << std::endl;
				return;
			}
			_vecEventCallBacks[type - 1](event);
		}

	private:
		std::vector<std::function<void(Event&)>> _vecEventCallBacks;
	};

}