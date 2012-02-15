#pragma once

#include "Event.h"

namespace KeyboardEventType
{
	enum KeyboardEventTypeEnum
	{
		Pressed,
		Released
	};
}

class KeyboardEvent
	: public Event
{
public:
	typedef KeyboardEventType::KeyboardEventTypeEnum type;

	const char key;
	const type eventType;

	KeyboardEvent(char key, KeyboardEvent::type eventType);
};