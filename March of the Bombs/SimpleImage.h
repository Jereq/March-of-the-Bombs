#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

class SimpleImage
{
private:
	TextureSection texture;
	glm::vec4 color;
	Rectanglef posSizeRectangle;
	float depth;

public:
	SimpleImage(TextureSection texture1, Rectanglef rectangle, float depth);
	SimpleImage(TextureSection texture1, Rectanglef rectangle, float depth, glm::vec4 color);
	virtual ~SimpleImage(void);
	void render(Graphics::ptr graphics);
};

