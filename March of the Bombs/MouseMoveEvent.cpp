#include "MouseMoveEvent.h"

MouseMoveEvent::MouseMoveEvent(glm::vec2 const& pos, bool pressed)
	: Event(EventType::MouseMove), position(pos), pressed(pressed)
{
}