#pragma once

#include <glm/glm.hpp>

class Frustum
{
private:
	glm::vec4 planes[6];

	static glm::vec4 getPlane(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3);
	static glm::vec4 normalizePlane(glm::vec4 const& plane);

public:
	enum PLANE_NUM
	{
		LEFT,
		RIGHT,
		BOTTOM,
		TOP,
		FRONT,
		BACK,
	};

	Frustum(
		glm::vec3 const& bottomLeftNear,
		glm::vec3 const& bottomRightNear,
		glm::vec3 const& topLeftNear,
		glm::vec3 const& topRightNear,
		glm::vec3 const& bottomLeftFar,
		glm::vec3 const& bottomRightFar,
		glm::vec3 const& topLeftFar,
		glm::vec3 const& topRightFar);
	Frustum(glm::mat4 const& viewProjectionMatrix);

	//Frustum mul(glm::mat4 const& matrix) const;

	glm::vec4 getLeftPlane() const;
	glm::vec4 getRightPlane() const;
	glm::vec4 getBottomPlane() const;
	glm::vec4 getTopPlane() const;
	glm::vec4 getFrontPlane() const;
	glm::vec4 getBackPlane() const;

	glm::vec4 getPlane(unsigned int num) const;
};