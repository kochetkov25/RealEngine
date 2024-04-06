#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>

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
			_vecEventCallBacks.emplace_back(base);
		}

		/*����� callback � ������������ � ����� event*/
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
		/*������, ������� ������ ��������*/
		std::vector<std::function<void(Event&)>> _vecEventCallBacks;
	};

}