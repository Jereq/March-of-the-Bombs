#pragma once

#include <glm/glm.hpp>
#include "Frustum.h"
#include "IntersectionResult.h"

class BoundingBox
{
public:
	glm::vec3 position;
	glm::vec3 halfSize;

	BoundingBox(glm::vec3 const& position, glm::vec3 const& halfSize);

	bool rayIntersect(glm::vec3 origin, glm::vec3 direction, float& distance) const;
	bool frustumIntersect(Frustum const& frustum);

	BoundingBox mul(glm::mat4 const& matrix) const;
};