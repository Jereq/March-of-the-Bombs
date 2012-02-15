#pragma once

#include "Event.h"
#include <glm/glm.hpp>

namespace MouseButton
{
	enum MouseButtonEnum
	{
		Left,
		Middle,
		Right
	};
}

namespace MouseButtonState
{
	enum MouseButtonStateEnum
	{
		Pressed,
		Released
	};
}

class MouseButtonEvent :
	public Event
{
public:
	typedef MouseButton::MouseButtonEnum mouseButton;
	typedef MouseButtonState::MouseButtonStateEnum mouseButtonState;

	const MouseButtonEvent::mouseButton button;
	const MouseButtonEvent::mouseButtonState state;
	const glm::vec2 position;

	MouseButtonEvent(MouseButtonEvent::mouseButton button, MouseButtonEvent::mouseButtonState state, glm::vec2 const& position);
};