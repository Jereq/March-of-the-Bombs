#pragma once

#include "Screen.h"
#include "Rectanglef.h"

class SimpleImage
{
private:
	GLTexture::ptr texture;
	glm::vec4 color;
	Rectanglef posSizeRectangle;

public:
	SimpleImage(GLTexture::ptr texture1, Rectanglef rectangle);
	SimpleImage(GLTexture::ptr texture1, Rectanglef rectangle, glm::vec4 color);
	virtual ~SimpleImage(void);
};

