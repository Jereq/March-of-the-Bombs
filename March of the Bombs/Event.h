#pragma once

#include <boost/shared_ptr.hpp>

namespace EventType
{
	/**
	 * Different types of events
	 */
	enum EventTypeEnum
	{
		Keyboard,
		MouseButton,
		MouseMove
	};
}

/**
 * Event is a base class for all events. To create a new event type, subclass Event
 * and add a name for the event to the EventType::EventTpyeEnum.
 */
class Event
{
public:
	typedef EventType::EventTypeEnum type;
	typedef boost::shared_ptr<Event> ptr;

protected:
	/// Constructor
	Event(Event::type eventType);

public:
	const Event::type eventType;

	virtual ~Event() {}
};