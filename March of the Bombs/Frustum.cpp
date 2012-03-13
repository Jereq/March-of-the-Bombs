#include "Frustum.h"

Frustum::Frustum()
{
}

glm::vec4 Frustum::getLeftPlane() const
{
	glm::vec3 p1p2 = TLF - TLN;
	glm::vec3 p1p3 = BLN - TLN;
	glm::vec3 Normal = glm::cross(p1p2, p1p3);
	float d = -((Normal.x * TLN.x) + (Normal.y * TLN.y) + (Normal.z * TLN.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getRightPlane() const
{
	glm::vec3 p1p2 = BRN - TRN;
	glm::vec3 p1p3 = TRF - TRN;
	glm::vec3 Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	float d = -((Normal.x * TRN.x) + (Normal.y * TRN.y) + (Normal.z * TRN.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getBottomPlane() const
{
	glm::vec3 p1p2 = BLF - BLN;
	glm::vec3 p1p3 = BRN - BLN;
	glm::vec3 Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	float d = -((Normal.x * BLN.x) + (Normal.y * BLN.y) + (Normal.z * BLN.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getTopPlane() const
{
	glm::vec3 p1p2 = TLF - TRF;
	glm::vec3 p1p3 = TRN - TRF;
	glm::vec3 Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	float d = -((Normal.x * TRF.x) + (Normal.y * TRF.y) + (Normal.z * TRF.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getBackPlane() const
{
	glm::vec3 p1p2 = TRF - TLF;
	glm::vec3 p1p3 = BLF - TLF;
	glm::vec3 Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	float d = -((Normal.x * TLF.x) + (Normal.y * TLF.y) + (Normal.z * TLF.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getFrontPlane() const
{
	glm::vec3 p1p2 = TLN - TRN;
	glm::vec3 p1p3 = BRN - TRN;
	glm::vec3 Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	float d = -((Normal.x * TRN.x) + (Normal.y * TRN.y) + (Normal.z * TRN.z));

	return glm::vec4(Normal, d);
}