#pragma once

#include "Event.h"

#include <glm/glm.hpp>

class MouseMoveEvent :
	public Event
{
public:
	const glm::vec2 position;
	const bool pressed;

	MouseMoveEvent(glm::vec2 const& pos, bool pressed);
};