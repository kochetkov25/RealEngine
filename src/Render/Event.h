#pragma once
#include <string>

namespace Render
{

	class Event
	{
	public:
		/*���� �������*/
		enum class EventType
		{
			MOUSE_MOVED
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

}