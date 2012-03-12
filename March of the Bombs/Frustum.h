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
	glm::vec4 planeEq;
	float a, b, c, d;

	Frustum();

	glm::vec4 getLeftPlane();
	glm::vec4 getRightPlane();
	glm::vec4 getBottomPlane();
	glm::vec4 getTopPlane();
	glm::vec4 getFrontPlane();
	glm::vec4 getBackPlane();
};