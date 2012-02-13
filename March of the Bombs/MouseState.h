#pragma once

#include <glm/glm.hpp>

namespace ButtonState
{
	enum ButtonStateEnum
	{
		Pressed,
		Released
	};
}

struct MouseState
{
public:
	MouseState();
	MouseState(ButtonState::ButtonStateEnum left,
		ButtonState::ButtonStateEnum right,
		ButtonState::ButtonStateEnum middle,
		glm::vec2 const& pos);

	ButtonState::ButtonStateEnum leftButton;
	ButtonState::ButtonStateEnum rightButton;
	ButtonState::ButtonStateEnum middleButton;
	glm::vec2 position;
};