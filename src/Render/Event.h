#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include <map>

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
			WINDOW_CLOSED,
			KEY_PRESSED,
			KEY_RELEASED,
			MOUSE_BUTTON_PRESSED,
			MOUSE_BUTTON_RELEASED,

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
			/*лямбда, которая принимает ссылку на базовый Event и даункастит до
			  ссылки на необходимый Event (для добавления коллбека в вектор)*/
			auto base = [func = std::move(callBack)](Event& e)
			{
				/*даункаст до ссылки на нужный объект*/
				func(static_cast<_typeEvent&>(e));
			};
			/*creating event to get event type*/
			_typeEvent e;
			_mapEventCallBacks.emplace(e.getType(), base);
		}

		/*вызов callback в соответствии с типои event*/
		void dispatch(Event& event)
		{
			auto it = _mapEventCallBacks.find(event.getType());
			if (it != _mapEventCallBacks.end())
				it->second(event);
			else
				std::cerr << __FUNCTION__ <<": Event not set!" << std::endl;
		}

	private:
		/*map для хранения callBack_ов*/
		std::map<Event::EventType, std::function<void(Event&)>> _mapEventCallBacks;
	};

}