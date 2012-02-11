#pragma once

#include <glm/glm.hpp>

class AttachmentPoint
{
private:
	glm::vec3 position;
	glm::vec3 rotation;

public:
	AttachmentPoint();
	AttachmentPoint(glm::vec3 position, glm::vec3 rotation);

	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getRotation() const;

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
};