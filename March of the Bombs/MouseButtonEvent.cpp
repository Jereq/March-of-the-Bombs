#include "MouseButtonEvent.h"

MouseButtonEvent::MouseButtonEvent(MouseButtonEvent::mouseButton button, MouseButtonEvent::mouseButtonState state, glm::vec2 const& position)
	: Event(EventType::MouseButton), button(button), state(state), position(position)
{
}