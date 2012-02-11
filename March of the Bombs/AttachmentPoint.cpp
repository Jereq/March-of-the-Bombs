#include "AttachmentPoint.h"

AttachmentPoint::AttachmentPoint()
{
}

AttachmentPoint::AttachmentPoint(glm::vec3 position, glm::vec3 rotation)
	: position(position), rotation(rotation)
{}

glm::vec3 AttachmentPoint::getPosition() const
{
	return position;
}

glm::vec3 AttachmentPoint::getRotation() const
{
	return rotation;
}

void AttachmentPoint::setPosition(glm::vec3 position)
{
	this->position = position;
}

void AttachmentPoint::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}