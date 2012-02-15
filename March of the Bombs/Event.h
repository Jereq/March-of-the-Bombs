#pragma once

#include <boost/shared_ptr.hpp>

namespace EventType
{
	enum EventTypeEnum
	{
		Keyboard,
		MouseButton
	};
}

class Event
{
public:
	typedef EventType::EventTypeEnum type;
	typedef boost::shared_ptr<Event> ptr;

protected:
	Event(Event::type eventType);

public:
	const Event::type eventType;

	virtual ~Event() {}
};