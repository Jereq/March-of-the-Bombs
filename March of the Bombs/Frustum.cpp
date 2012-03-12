#include "Frustum.h"

Frustum::Frustum()
{
}

glm::vec3 Frustum::getLeftPlane()
{
	p1p2 = TLF - TLN;
	p1p3 = BLN - TLN;
	Normal = glm::cross(p1p2, p1p3);
}

glm::vec3 Frustum::getRightPlane()
{
	p1p2 = TRF - TRN;
	p1p3 = BRN - TRN;
	Normal = glm::cross(p1p2, p1p3);
}

glm::vec3 Frustum::getLowerPlane()
{
	p1p2 = BLF - BLN;
	p1p3 = BRN - BLN;
	Normal = glm::cross(p1p2, p1p3);
}

glm::vec3 Frustum::getTopPlane()
{
	p1p2 = TLF - TLN;
	p1p3 = TRN - TLN;
	Normal = glm::cross(p1p2, p1p3);
}

glm::vec3 Frustum::getFrontPlane()
{
	p1p2 = TRN - TLN;
	p1p3 = BLN - TLN;
	Normal = glm::cross(p1p2, p1p3);
}

glm::vec3 Frustum::getBackPlane()
{
	p1p2 = TRF - TLF;
	p1p3 = BLN - TLF;
	Normal = glm::cross(p1p2, p1p3);
}