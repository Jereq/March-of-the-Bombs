#include "PointLight.h"

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

#include <glm/gtc/matrix_transform.hpp>
using glm::perspective;
using glm::lookAt;

mat4 const& PointLight::getProjectionMatrix() const
{
	if (!validProjectionMatrix)
	{
		const static float fieldOfViewY = 45.f;
		const static float aspect = 1.f;
		const static float zNear = 1.f;
		const static float zFar = 500.f;

		projectionMatrix = perspective(fieldOfViewY, aspect, zNear, zFar);

		validProjectionMatrix = true;
	}

	return projectionMatrix;
}

mat4 const& PointLight::getViewMatrix() const
{
	if (!validViewMatrix)
	{
		const static vec3 direction = vec3(0, -1, 0);
		const static vec3 up = vec3(0, 0, -1);

		viewMatrix = lookAt(vec3(getPosition()), vec3(getPosition()) + direction, up);

		validViewMatrix = true;
	}

	return viewMatrix;
}

GLuint PointLight::getShadowTexture(GLsizei size, GLuint& framebuffer)
{
	GLfloat border[] = { 1.f, 0, 0, 0 };

	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);


	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return depthTex;
}

const static GLsizei shadowSize = 2048;

PointLight::PointLight()
	: validProjectionMatrix(false), validViewMatrix(false), validViewProjectionMatrix(false)
{
	shadowResolution = shadowSize;
	shadowTexture = getShadowTexture(shadowResolution, shadowBuffer);
}

PointLight::PointLight(vec4 position, vec3 intensity)
	: position(position), intensity(intensity),
	 validProjectionMatrix(false), validViewMatrix(false), validViewProjectionMatrix(false)
{
	shadowResolution = shadowSize;
	shadowTexture = getShadowTexture(shadowResolution, shadowBuffer);
}

PointLight::~PointLight()
{
	glDeleteFramebuffers(1, &shadowBuffer);
	glDeleteTextures(1, &shadowTexture);
}

void PointLight::setPosition(vec4 const& position)
{
	this->position = position;

	validViewMatrix = false;
	validViewProjectionMatrix = false;
}

void PointLight::setIntensity(vec3 const& intestity)
{
	this->intensity = intestity;
}

vec4 const& PointLight::getPosition() const
{
	return position;
}

vec3 const& PointLight::getIntensity() const
{
	return intensity;
}

glm::mat4 const& PointLight::getViewProjectionMatrix() const
{
	if (!validViewProjectionMatrix)
	{
		const static glm::mat4 bias = glm::mat4(
			.5f,  0 ,  0 , 0,
			 0 , .5f,  0 , 0,
			 0 ,  0 , .5f, 0,
			.5f, .5f, .5f, 1.f);

		viewProjectionMatrix = bias * getProjectionMatrix() * getViewMatrix();
	}

	return viewProjectionMatrix;
}

GLuint PointLight::getShadowTexture() const
{
	return shadowTexture;
}

GLuint PointLight::getShadowBuffer() const
{
	return shadowBuffer;
}

GLsizei PointLight::getShadowResolution() const
{
	return shadowResolution;
}