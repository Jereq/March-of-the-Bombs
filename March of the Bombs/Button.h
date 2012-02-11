#pragma once

#include "GLTexture.h"
#include <glm\glm.hpp>

class Button
{
private:
	GLTexture::ptr unpressedTexture;		//storing buttons unpressedtexture under a name
	GLTexture::ptr pressedTexture;			//storing buttons pressedtexture under a name
	glm::vec4 color;						//using vec4 for more options later on, this as color

public:
	Button();
	virtual ~Button();
};