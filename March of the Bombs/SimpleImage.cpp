#include "SimpleImage.h"


SimpleImage::SimpleImage(GLTexture::ptr texture, Rectanglef rectangle)
	: texture(texture), posSizeRectangle(rectangle), color(glm::vec4(1))
{

}

SimpleImage::SimpleImage(GLTexture::ptr texture, Rectanglef rectangle, glm::vec4 color)
	: texture(texture), posSizeRectangle(rectangle), color(color)
{

}

SimpleImage::~SimpleImage(void)
{
}

void SimpleImage::render(Graphics::ptr graphics)
{
	graphics->drawTexture(texture,posSizeRectangle);
}