#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;

Camera::Camera(AttachmentPoint::ptr attachmentPoint)
	: projectionMatrix(), viewMatrix(), attachmentPoint(attachmentPoint)
{
	updateProjectionMatrix(1);
	updateViewMatrix();
}

void Camera::updateProjectionMatrix(float aspect)
{
	const float nearClip = 0.1f;
	const float farClip = 10000.f;
	const float fieldOfViewY_deg = 45.f;

	projectionMatrix = glm::perspective(fieldOfViewY_deg, aspect, nearClip, farClip);
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

	viewMatrix = glm::lookAt(position, position + forwardDirection, glm::cross(rightDirection, forwardDirection));
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMatrix;
}

AttachmentPoint::ptr const& Camera::getAttachmentPoint() const
{
	return attachmentPoint;
}

void Camera::setAttachmentPoint(AttachmentPoint::ptr attachmentPoint)
{
	this->attachmentPoint = attachmentPoint;
}