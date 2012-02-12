#include "SimpleImage.h"


SimpleImage(GLTexture::ptr texture1, Rectanglef rectangle);
: texture(texture1), posSizeRectangle(rectangle), color(glm::vec4(1))
{

}

SimpleImage(GLTexture::ptr texture1, Rectanglef rectangle, glm::vec4 color);
{

}

SimpleImage::~SimpleImage(void)
{
}
