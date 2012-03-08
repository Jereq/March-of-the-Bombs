#pragma once

#include <glm/glm.hpp>

class BoundingBox
{
public:
	glm::vec3 position;
	glm::vec3 halfSize;

	BoundingBox(glm::vec3 const& position, glm::vec3 const& halfSize);

	bool rayIntersect(glm::vec3 origin, glm::vec3 direction, float& distance) const;

	//// TLN = TopLeftNear, BRN = BottomRightNear
	//// TLF = TopLeftFar, BRF = BottomRightFar
	//void selectionBox(glm::vec3 TLN, glm::vec3 BRN, glm::vec3 TLF, glm::vec3 BRF, BoundingBox& BoundingBox);
};