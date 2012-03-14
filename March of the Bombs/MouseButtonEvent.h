#pragma once

#include "Event.h"
#include <glm/glm.hpp>

namespace MouseButton
{
	/*
	 * Different buttons on the mouse
	 */
	enum MouseButtonEnum
	{
		Left,
		Middle,
		Right
	};
}

namespace MouseButtonState
{
	/*
	 * The different actions of a mouse button
	 */
	enum MouseButtonStateEnum
	{
		Pressed,
		Released
	};
}

/*
 * A MouseButtonEvent represents the the action of a mouse button being pressed or released
 */
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