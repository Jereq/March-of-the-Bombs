#include "Button.h"

bool pressed = false;

//constructor without changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectanglef rectangle, float depth)
	: unpressedTexture(texture1), pressedTexture(texture2), posSizeRectangle(rectangle), depth(depth), color(glm::vec4(1)), buttonState(Unused)
{

}

//constructor changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectanglef rectangle, float depth, glm::vec4 color)
	: unpressedTexture(texture1), pressedTexture(texture2),posSizeRectangle(rectangle), depth(depth), color(color), buttonState(Unused)
{

}

Button::~Button()
{
}

void Button::render(Graphics::ptr graphics)
{
	switch (buttonState)
	{
	case Unused:
		{
			graphics->drawTexture(unpressedTexture,posSizeRectangle, depth);
		}
		break;

	case Hovered:
		{
			graphics->drawTexture(pressedTexture,posSizeRectangle, depth);
		}
		break;

	case Used:
		{
			graphics->drawTexture(pressedTexture,posSizeRectangle, depth);
		}
		break;
	}
}

void Button::setState(ButtonState bs)
{
	buttonState = bs;
}

ButtonState Button::getState()
{
	return buttonState;
}


bool Button::intersects(glm::vec2 const& point) const
{
	return posSizeRectangle.intersects(point);
}

bool Button::isPressed() const
{
	return pressed;
}
