#include "Button.h"

//constructor without changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2)
	: unpressedTexture(texture1), pressedTexture(texture2), color(glm::vec4(1))
{

}

//constructor changing the vec4 color
Button::Button(GLTexture::ptr texture1, GLTexture::ptr texture2, glm::vec4 color)
	: unpressedTexture(texture1), pressedTexture(texture2), color(color)
{

}

Button::~Button()
{
}

void render()
{

}