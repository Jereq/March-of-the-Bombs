#include "Frustum.h"

glm::vec4 Frustum::getPlane(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3)
{
	glm::vec3 p1p2 = p2 - p1;
	glm::vec3 p1p3 = p3 - p1;
	glm::vec3 normal = glm::cross(p1p2, p1p3);
	normal = glm::normalize(normal);
	float d = -((normal.x * p1.x) + (normal.y * p1.y) + (normal.z * p1.z));

	return glm::vec4(normal, d);
}

glm::vec4 Frustum::normalizePlane(glm::vec4 const& plane)
{
	float mag = glm::length(glm::vec3(plane));

	return plane / mag;
}

Frustum::Frustum(
		glm::vec3 const& bottomLeftNear,
		glm::vec3 const& bottomRightNear,
		glm::vec3 const& topLeftNear,
		glm::vec3 const& topRightNear,
		glm::vec3 const& bottomLeftFar,
		glm::vec3 const& bottomRightFar,
		glm::vec3 const& topLeftFar,
		glm::vec3 const& topRightFar)
{
	planes[LEFT] = getPlane(topLeftNear, topLeftFar, bottomLeftNear);
	planes[RIGHT] = getPlane(topRightNear, bottomRightNear, topRightFar);
	planes[BOTTOM] = getPlane(bottomLeftNear, bottomLeftFar, bottomRightNear);
	planes[TOP] = getPlane(topRightFar, topLeftFar, topRightNear);
	planes[BACK] = getPlane(topLeftFar, topRightFar, bottomLeftFar);
	planes[FRONT] = getPlane(topRightNear, topLeftFar, bottomRightNear);
}

Frustum::Frustum(glm::mat4 const& viewProjectionMatrix)
{
	glm::mat4 transpMat = glm::transpose(viewProjectionMatrix);
	glm::vec4 const& row0 = transpMat[0];
	glm::vec4 const& row1 = transpMat[1];
	glm::vec4 const& row2 = transpMat[2];
	glm::vec4 const& row3 = transpMat[3];

	planes[LEFT] = normalizePlane(-(row3 + row0));
	planes[RIGHT] = normalizePlane(-(row3 - row0));
	planes[BOTTOM] = normalizePlane(-(row3 + row1));
	planes[TOP] = normalizePlane(-(row3 - row1));
	planes[FRONT] = normalizePlane(-(row3 + row2));
	planes[BACK] = normalizePlane(-(row3 - row2));
}

glm::vec4 Frustum::getLeftPlane() const
{
	return planes[LEFT];
}

glm::vec4 Frustum::getRightPlane() const
{
	return planes[RIGHT];
}

glm::vec4 Frustum::getBottomPlane() const
{
	return planes[BOTTOM];
}

glm::vec4 Frustum::getTopPlane() const
{
	return planes[TOP];
}

glm::vec4 Frustum::getBackPlane() const
{
	return planes[BACK];
}

glm::vec4 Frustum::getFrontPlane() const
{
	return planes[FRONT];
}

glm::vec4 Frustum::getPlane(unsigned int num) const
{
	assert(num < 6);

	return planes[num];
}