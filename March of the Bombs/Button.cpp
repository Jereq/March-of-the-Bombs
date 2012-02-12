#include "Button.h"

bool pressed = false;

//constructor without changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectangle rectangle)
	: unpressedTexture(texture1), pressedTexture(texture2), posSizeRectangle(rectangle), color(glm::vec4(1))
{

}

//constructor changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectangle rectangle, glm::vec4 color)
	: unpressedTexture(texture1), pressedTexture(texture2),posSizeRectangle(rectangle), color(color)
{

}

Button::~Button()
{
}

void Button::render(Graphics::ptr dT)
{
	if(!pressed)
	{
		dT->drawTexture(unpressedTexture);
	}
	else
	{
		dT->drawTexture(pressedTexture);
	}

}