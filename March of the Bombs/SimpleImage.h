#pragma once

#include "GLTexture.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

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
	void render(Graphics::ptr graphics);
};

