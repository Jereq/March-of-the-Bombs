#pragma once

#include <glm/glm.hpp>

#include "AttachmentPoint.h"

class Camera
{
private:
	AttachmentPoint const* attachmentPoint;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

public:
	Camera(AttachmentPoint const* attachmentPoint);

	void updateProjectionMatrix(float aspect);
	void updateViewMatrix();

	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;

	void setAttachmentPoint(AttachmentPoint const* attachmentPoint);
};