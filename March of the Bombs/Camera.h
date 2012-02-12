#pragma once

#include <glm/glm.hpp>

#include <boost/shared_ptr.hpp>

#include "AttachmentPoint.h"

class Camera
{
private:
	AttachmentPoint::ptr attachmentPoint;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

public:
	typedef boost::shared_ptr<Camera> ptr;

	Camera(AttachmentPoint::ptr attachmentPoint);

	void updateProjectionMatrix(float aspect);
	void updateViewMatrix();

	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;

	void setAttachmentPoint(AttachmentPoint::ptr attachmentPoint);
};