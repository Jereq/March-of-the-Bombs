#pragma once

#include "GLTexture.h"
#include <glm\glm.hpp>
//#include "Rectagle.h"

class Button
{
private:
	GLTexture::ptr unpressedTexture;		//storing buttons unpressedtexture under a name
	GLTexture::ptr pressedTexture;			//storing buttons pressedtexture under a name
	glm::vec4 color;						//using vec4 for more options later on, this as color
	//RECTANGLE

public:
	Button(GLTexture::ptr texture1, GLTexture::ptr texture2);
	Button(GLTexture::ptr texture1, GLTexture::ptr texture2, glm::vec4 color);
	virtual ~Button();
	void render();
};