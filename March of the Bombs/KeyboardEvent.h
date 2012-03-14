#pragma once

#include "Event.h"

namespace KeyboardEventType
{
	/**
	 * A key can be pressed or released
	 */
	enum KeyboardEventTypeEnum
	{
		Pressed,
		Released
	};
}

/*
 * A keyboard event represents the action of a key being pressed or released
 */
class KeyboardEvent
	: public Event
{
public:
	typedef KeyboardEventType::KeyboardEventTypeEnum type;

	const char key;
	const type keyEventType;

	KeyboardEvent(char key, KeyboardEvent::type eventType);
};