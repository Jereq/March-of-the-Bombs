#include "SimpleImage.h"


SimpleImage::SimpleImage(TextureSection texture, Rectanglef rectangle, float depth)
	: texture(texture), posSizeRectangle(rectangle),depth(depth), color(glm::vec3(1))
{

}

SimpleImage::SimpleImage(TextureSection texture, Rectanglef rectangle, float depth, glm::vec3 color)
	: texture(texture), posSizeRectangle(rectangle), depth(depth), color(color)
{

}

SimpleImage::~SimpleImage(void)
{
}

void SimpleImage::render(Graphics::ptr graphics)
{
	graphics->drawTexture(texture,posSizeRectangle, depth, color);
}