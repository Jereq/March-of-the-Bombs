#include "Button.h"

bool pressed = false;

//constructor without changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectanglef rectangle, float depth)
	: unpressedTexture(texture1), pressedTexture(texture2), posSizeRectangle(rectangle), depth(depth), color(glm::vec4(1)), pressed(false)
{

}

//constructor changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectanglef rectangle, float depth, glm::vec4 color)
	: unpressedTexture(texture1), pressedTexture(texture2),posSizeRectangle(rectangle), depth(depth), color(color), pressed(false)
{

}

Button::~Button()
{
}

void Button::render(Graphics::ptr graphics)
{
	if(!pressed)
	{
		graphics->drawTexture(unpressedTexture,posSizeRectangle, depth);
	}
	else
	{
		graphics->drawTexture(pressedTexture,posSizeRectangle, depth);
	}
}

void Button::changeState()
{
	if(!pressed)
	{
		pressed = true;
	}
	else
	{
		pressed = false;
	}
}

bool Button::intersects(glm::vec2 const& point) const
{
	return posSizeRectangle.intersects(point);
}

bool Button::isPressed() const
{
	return pressed;
}