#include "OrientedBoundingBox.h"

OrientedBoundingBox::OrientedBoundingBox(BoundingBox const& alignedBoundingBox)
{
	center = alignedBoundingBox.position;

	directions[0] = glm::vec3(1, 0, 0);
	directions[1] = glm::vec3(0, 1, 0);
	directions[2] = glm::vec3(0, 0, 1);

	halfSizes = alignedBoundingBox.halfSize;
}

OrientedBoundingBox::OrientedBoundingBox(BoundingBox const& alignedBoundingBox, glm::mat4 const& matrix)
{
	center = glm::vec3(matrix * glm::vec4(alignedBoundingBox.position, 1.f));

	directions[0] = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
	directions[1] = glm::vec3(matrix * glm::vec4(0, 1, 0, 0));
	directions[2] = glm::vec3(matrix * glm::vec4(0, 0, 1, 0));
	
	halfSizes[0] = glm::length(directions[0]);
	halfSizes[1] = glm::length(directions[1]);
	halfSizes[2] = glm::length(directions[2]);

	directions[0] /= halfSizes[0];
	directions[1] /= halfSizes[1];
	directions[2] /= halfSizes[2];

	halfSizes[0] *= alignedBoundingBox.halfSize[0];
	halfSizes[1] *= alignedBoundingBox.halfSize[1];
	halfSizes[2] *= alignedBoundingBox.halfSize[2];
}

OrientedBoundingBox::OrientedBoundingBox(glm::vec3 const& center, glm::vec3 const& halfSize, glm::mat4 const& matrix)
{
	this->center = glm::vec3(matrix * glm::vec4(center, 1.f));

	directions[0] = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
	directions[1] = glm::vec3(matrix * glm::vec4(0, 1, 0, 0));
	directions[2] = glm::vec3(matrix * glm::vec4(0, 0, 1, 0));
	
	halfSizes[0] = glm::length(directions[0]);
	halfSizes[1] = glm::length(directions[1]);
	halfSizes[2] = glm::length(directions[2]);

	directions[0] /= halfSizes[0];
	directions[1] /= halfSizes[1];
	directions[2] /= halfSizes[2];

	halfSizes[0] *= halfSize[0];
	halfSizes[1] *= halfSize[1];
	halfSizes[2] *= halfSize[2];
}

IntersectionResult::Value OrientedBoundingBox::planeIntersect(glm::vec4 const& plane) const
{
	glm::vec3 normal(plane);

	float extent =
		halfSizes[0] * glm::abs(glm::dot(normal, directions[0])) +
		halfSizes[1] * glm::abs(glm::dot(normal, directions[1])) +
		halfSizes[2] * glm::abs(glm::dot(normal, directions[2]));

	float centerDistance = glm::dot(center, normal) + plane[3];

	if (centerDistance - extent > 0)
	{
		return IntersectionResult::OUTSIDE;
	}

	if (centerDistance + extent < 0)
	{
		return IntersectionResult::INSIDE;
	}

	return IntersectionResult::INTERSECTING;
}

IntersectionResult::Value OrientedBoundingBox::frustumIntersect(Frustum const& frustum) const
{
	bool intersecting = false;

	for (int i = 0; i < 6; i++)
	{
		IntersectionResult::Value result = planeIntersect(frustum.getPlane(i));

		if (result == IntersectionResult::OUTSIDE)
		{
			return IntersectionResult::OUTSIDE; 
		}
		else if (result == IntersectionResult::INTERSECTING)
		{
			intersecting = true;
		}
	}

	if (intersecting)
	{
		return IntersectionResult::INTERSECTING;
	}

	return IntersectionResult::INSIDE;
}