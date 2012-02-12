#include "Rectanglef.h"

#include <glm/glm.hpp>
using glm::vec2;

Rectanglef::Rectanglef()
{
}

Rectanglef::Rectanglef(vec2 const& position, vec2 const& size)
	: position(position), size(size)
{
}

vec2 Rectanglef::getPosition() const
{
	return position;
}

vec2 Rectanglef::getSize() const
{
	return size;
}

void Rectanglef::setPosition(vec2 const& newPos)
{
	position = newPos;
}

void Rectanglef::setSize(vec2 const& newSize)
{
	size = newSize;
}