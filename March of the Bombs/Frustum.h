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

	Frustum();
	Frustum(
		glm::vec3 const& bottomLeftNear,
		glm::vec3 const& bottomRightNear,
		glm::vec3 const& topLeftNear,
		glm::vec3 const& topRightNear,
		glm::vec3 const& bottomLeftFar,
		glm::vec3 const& bottomRightFar,
		glm::vec3 const& topLeftFar,
		glm::vec3 const& topRightFar);

	Frustum mul(glm::mat4 const& matrix) const;

	glm::vec4 getLeftPlane() const;
	glm::vec4 getRightPlane() const;
	glm::vec4 getBottomPlane() const;
	glm::vec4 getTopPlane() const;
	glm::vec4 getFrontPlane() const;
	glm::vec4 getBackPlane() const;
};