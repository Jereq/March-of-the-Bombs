#pragma once

#include "GLTexture.h"
#include "Rectangle.h"
#include "Graphics.h"
#include <glm\glm.hpp>

class Button
{
private:
	GLTexture::ptr unpressedTexture;		//storing buttons unpressedtexture under a name
	GLTexture::ptr pressedTexture;			//storing buttons pressedtexture under a name
	glm::vec4 color;						//using vec4 for more options later on, this as color
	Rectangle posSizeRectangle;				//position- and size- rectangle 
	bool pressed;

public:
	Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectangle rectangle);
	Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectangle rectangle, glm::vec4 color);
	virtual ~Button();
	void render(Graphics::ptr dT);
};