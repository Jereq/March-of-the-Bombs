#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;

Camera::Camera(AttachmentPoint const* attachmentPoint)
	: projectionMatrix(), viewMatrix(), attachmentPoint(attachmentPoint)
{
	updateProjectionMatrix(1);
	updateViewMatrix();
}

glm::mat4 perspective(float feildOfViewY_deg, float aspectRatio, float nearClip, float farClip)
{
	float range = tan(glm::radians(feildOfViewY_deg / 2)) * nearClip;	
	float left = -range * aspectRatio;
	float right = range * aspectRatio;
	float bottom = -range;
	float top = range;

	glm::mat4 Result(0);
	Result[0][0] = (2 * nearClip) / (right - left);
	Result[1][1] = (2 * nearClip) / (top - bottom);
	Result[2][2] = - (farClip + nearClip) / (farClip - nearClip);
	Result[2][3] = - 1;
	Result[3][2] = - (2 * farClip * nearClip) / (farClip - nearClip);
	return Result;
}

void Camera::updateProjectionMatrix(float aspect)
{
	const float nearClip = 0.1f;
	const float farClip = 10000.f;
	const float fieldOfViewY_deg = 45.f;

	projectionMatrix = perspective(fieldOfViewY_deg, aspect, nearClip, farClip);
}

glm::mat4 lookAt(vec3 position, vec3 target, vec3 up)
{
	vec3 f = glm::normalize(target - position);
	vec3 u = glm::normalize(up);
	vec3 s = glm::normalize(glm::cross(f, u));
	u = glm::cross(s, f);

	glm::mat4 Result(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] =-f.x;
	Result[1][2] =-f.y;
	Result[2][2] =-f.z;
/*  Test this instead of translate3D
	Result[3][0] =-dot(s, eye);
	Result[3][1] =-dot(y, eye);
	Result[3][2] = dot(f, eye);
*/  
	return glm::translate(Result, -position);
}

void Camera::updateViewMatrix()
{
	vec3 position = attachmentPoint->getPosition();
	vec3 rotation = attachmentPoint->getRotation();

	glm::mat3 rotateX = glm::mat3(glm::rotate(glm::mat4(), rotation.x, glm::vec3(1, 0, 0)));
	glm::mat3 rotateY = glm::mat3(glm::rotate(glm::mat4(), rotation.y, glm::vec3(0, 1, 0)));
	glm::mat3 rotateZ = glm::mat3(glm::rotate(glm::mat4(), rotation.z, glm::vec3(0, 0, 1)));

	glm::vec3 forwardDirection = rotateY * rotateX * glm::vec3(0, 0, -1);
	glm::vec3 rightDirection = rotateY * glm::vec3(1, 0, 0);

	viewMatrix = lookAt(position, position + forwardDirection, glm::cross(rightDirection, forwardDirection));
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMatrix;
}

void Camera::setAttachmentPoint(AttachmentPoint const* attachmentPoint)
{
	this->attachmentPoint = attachmentPoint;
}