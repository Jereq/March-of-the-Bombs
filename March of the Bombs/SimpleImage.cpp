#include "SimpleImage.h"


SimpleImage::SimpleImage(TextureSection texture, Rectanglef rectangle, float depth)
	: texture(texture), posSizeRectangle(rectangle),depth(depth), color(glm::vec4(1))
{

}

SimpleImage::SimpleImage(TextureSection texture, Rectanglef rectangle, float depth, glm::vec4 color)
	: texture(texture), posSizeRectangle(rectangle), depth(depth), color(color)
{

}

SimpleImage::~SimpleImage(void)
{
}

void SimpleImage::render(Graphics::ptr graphics)
{
	graphics->drawTexture(texture,posSizeRectangle, depth, glm::vec3(1.f));
}