#include "MouseState.h"

MouseState::MouseState()
	: leftButton(ButtonState::Released), rightButton(ButtonState::Released),
	middleButton(ButtonState::Released)
{
}

MouseState::MouseState(ButtonState::ButtonStateEnum left,
		ButtonState::ButtonStateEnum right,
		ButtonState::ButtonStateEnum middle,
		glm::vec2 const& pos)
		: leftButton(left), rightButton(right), middleButton(middle), position(pos)
{
}