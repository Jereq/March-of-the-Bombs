#include "Rectangle.h"

#include <glm/glm.hpp>
using glm::vec2;

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(vec2 const& position, vec2 const& size)
	: position(position), size(size)
{
}

vec2 Rectangle::getPosition() const
{
	return position;
}

vec2 Rectangle::getSize() const
{
	return size;
}

void Rectangle::setPosition(vec2 const& newPos)
{
	position = newPos;
}

void Rectangle::setSize(vec2 const& newSize)
{
	size = newSize;
}