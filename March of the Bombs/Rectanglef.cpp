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

bool Rectanglef::intersects(vec2 const& point) const
{
	if (point.x < position.x ||
		point.y < position.y ||
		point.x > position.x + size.x ||
		point.y > position.y + size.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}