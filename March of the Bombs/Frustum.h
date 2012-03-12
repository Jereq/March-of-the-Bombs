#pragma once

#include <glm/glm.hpp>

class Frustum
{
public:
	// TLN = TopLeftNear
	glm::vec3 TLN;

	// TRN = TopRightNear
	glm::vec3 TRN;

	// TLF = TopLeftFar
	glm::vec3 TLF;

	// TRF = TopRightFar
	glm::vec3 TRF;

	// BLN = BottomLeftNear
	glm::vec3 BLN;

	// BRN = BottomRightNear
	glm::vec3 BRN;

	// BLF = BottomLeftFar
	glm::vec3 BLF;

	// BRF = BottomRightFar
	glm::vec3 BRF;

	glm::vec3 p1p2, p1p3, Normal;

	Frustum();

	glm::vec3 getLeftPlane();
	glm::vec3 getRightPlane();
	glm::vec3 getLowerPlane();
	glm::vec3 getTopPlane();
	glm::vec3 getFrontPlane();
	glm::vec3 getBackPlane();
};