#include "Frustum.h"

Frustum::Frustum()
{
}

glm::vec4 Frustum::getLeftPlane()
{
	p1p2 = TLF - TLN;
	p1p3 = BLN - TLN;
	Normal = glm::cross(p1p2, p1p3);
	d = -((Normal.x * TLN.x) + (Normal.y * TLN.y) + (Normal.z * TLN.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getRightPlane()
{
	p1p2 = BRN - TRN;
	p1p3 = TRF - TRN;
	Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	d = -((Normal.x * TRN.x) + (Normal.y * TRN.y) + (Normal.z * TRN.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getBottomPlane()
{
	p1p2 = BLF - BLN;
	p1p3 = BRN - BLN;
	Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	d = -((Normal.x * BLN.x) + (Normal.y * BLN.y) + (Normal.z * BLN.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getTopPlane()
{
	p1p2 = TLF - TRF;
	p1p3 = TRN - TRF;
	Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	d = -((Normal.x * TRF.x) + (Normal.y * TRF.y) + (Normal.z * TRF.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getFrontPlane()
{
	p1p2 = TLN - TRN;
	p1p3 = BRN - TRN;
	Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	d = -((Normal.x * TRN.x) + (Normal.y * TRN.y) + (Normal.z * TRN.z));

	return glm::vec4(Normal, d);
}

glm::vec4 Frustum::getBackPlane()
{
	p1p2 = TRF - TLF;
	p1p3 = BLF - TLF;
	Normal = glm::cross(p1p2, p1p3);
	// Insert point 1 for d
	d = -((Normal.x * TLF.x) + (Normal.y * TLF.y) + (Normal.z * TLF.z));

	return glm::vec4(Normal, d);
}