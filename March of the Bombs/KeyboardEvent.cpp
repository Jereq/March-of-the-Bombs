#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent(char key, KeyboardEvent::type eventType)
	: Event(EventType::Keyboard), key(key), eventType(eventType)
{
}