#pragma once

#include "BoundingBox.h"

class OrientedBoundingBox
{
private:
	glm::vec3 center;
	glm::vec3 directions[3];
	glm::vec3 halfSizes;

public:
	OrientedBoundingBox(BoundingBox const& alignedBoundingBox);
	OrientedBoundingBox(BoundingBox const& alignedBoundingBox, glm::mat4 const& matrix);
	OrientedBoundingBox(glm::vec3 const& center, glm::vec3 const& halfSize, glm::mat4 const& matrix);

	IntersectionResult::Value planeIntersect(glm::vec4 const& plane) const;
	IntersectionResult::Value frustumIntersect(Frustum const& frustum) const;
};