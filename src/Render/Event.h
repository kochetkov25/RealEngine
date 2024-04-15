#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include <map>

namespace Render
{
	/*������� ����� ��� �������� �������*/
	class Event
	{
	public:
		/*���� �������*/
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

		/*��� �������*/
		inline std::string getName() const { return _name; }

		/*��� �������*/
		inline EventType getType() const { return _type; }

		/*����� ����������� ������� ��� ������ ���������� � �������*/
		virtual std::string format() const = 0;
	protected:
		EventType _type;
		std::string _name;

		Event(const std::string& name, const EventType type)
			:
			_type(type),
			_name(name) {}
	};



	/*��������� �������*/
	class EventDispatcher
	{
	public:
		template<typename _typeEvent>
		void addEventListner(std::function<void(_typeEvent&)> callBack)
		{
			/*������, ������� ��������� ������ �� ������� Event � ���������� ��
			  ������ �� ����������� Event (��� ���������� �������� � ������)*/
			auto base = [func = std::move(callBack)](Event& e)
			{
				/*�������� �� ������ �� ������ ������*/
				func(static_cast<_typeEvent&>(e));
			};
			/*creating event to get event type*/
			_typeEvent e;
			_mapEventCallBacks.emplace(e.getType(), base);
		}

		/*����� callback � ������������ � ����� event*/
		void dispatch(Event& event)
		{
			auto it = _mapEventCallBacks.find(event.getType());
			if (it != _mapEventCallBacks.end())
				it->second(event);
			else
				std::cerr << __FUNCTION__ <<": Event not set!" << std::endl;
		}

	private:
		/*map ��� �������� callBack_��*/
		std::map<Event::EventType, std::function<void(Event&)>> _mapEventCallBacks;
	};

}