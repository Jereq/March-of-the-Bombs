#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

class SimpleImage
{
private:
	TextureSection texture;
	glm::vec3 color;
	Rectanglef posSizeRectangle;
	float depth;

public:
	typedef boost::shared_ptr<SimpleImage> ptr;

	SimpleImage(TextureSection texture1, Rectanglef rectangle, float depth);
	SimpleImage(TextureSection texture1, Rectanglef rectangle, float depth, glm::vec3 color);
	virtual ~SimpleImage(void);
	void render(Graphics::ptr graphics);
};

