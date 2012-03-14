#pragma once

#include "Event.h"

#include <glm/glm.hpp>

/*
 * A MouseMoveEvent represents the action of the mouse moving
 */
class MouseMoveEvent :
	public Event
{
public:
	/// The new position of the mouse, 0 to 1, with the origin in the lower left corner of the window
	const glm::vec2 position;

	/// Whether the mouse was moved while a button was pressed or not
	const bool pressed;

	MouseMoveEvent(glm::vec2 const& pos, bool pressed);
};