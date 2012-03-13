#include "PointLight.h"

#include <boost/foreach.hpp>

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

#include <glm/gtc/matrix_transform.hpp>
using glm::perspective;
using glm::lookAt;

void PointLight::updateViewMatrix() const
{
	vec3 targ;
	if (target.w == 0)
	{
		targ = vec3(position + target);
	}
	else
	{
		targ = vec3(target);
	}

	vec3 direction = targ - vec3(position);

	const static vec3 ex(1, 0, 0);
	const static vec3 ey(0, 1, 0);

	vec3 n1 = glm::cross(direction, ex);
	vec3 n2 = glm::cross(direction, ey);

	vec3 up;

	if (glm::length(n1) > glm::length(n2))
	{
		up = glm::normalize(n1);
	}
	else
	{
		up = glm::normalize(n2);
	}

	viewMatrix = lookAt(vec3(getPosition()), targ, up);

	validViewMatrix = true;
}

void PointLight::updateProjectionMatrix() const
{
	if (points.empty())
	{
		return;
	}

	float nearClip(std::numeric_limits<float>::max());
	float farClip(std::numeric_limits<float>::min());
	float lengthX = 0.f;
	float lengthY = 0.f;

	glm::mat4 viewMatrix = getViewMatrix();

	BOOST_FOREACH(glm::vec3 const& worldPoint, points)
	{
		glm::vec3 eyePoint(viewMatrix * glm::vec4(worldPoint, 1.f));

		if (-eyePoint.z < std::numeric_limits<float>::min())
		{
			continue;
		}

		if (-eyePoint.z < nearClip)
		{
			nearClip = -eyePoint.z;
		}

		if (-eyePoint.z > farClip)
		{
			farClip = -eyePoint.z;
		}

		float leX = glm::abs(eyePoint.x / eyePoint.z);
		float leY = glm::abs(eyePoint.y / eyePoint.z);

		if (leX > lengthX)
		{
			lengthX = leX;
		}

		if (leY > lengthY)
		{
			lengthY = leY;
		}
	}

	float aspect = lengthY / lengthX;
	float fieldOfViewY_deg = glm::degrees(glm::atan(lengthY)) * 2.f;

	const static float MOD_NEAR_CLIP = 0.f;
	const static float MOD_FAR_CLIP = 0.1f;

	float clipDistance = farClip - nearClip;

	projectionMatrix = glm::perspective(fieldOfViewY_deg, aspect, nearClip - clipDistance * MOD_NEAR_CLIP, farClip + clipDistance * MOD_FAR_CLIP);

	validProjectionMatrix = true;
}

mat4 const& PointLight::getProjectionMatrix() const
{
	if (!validProjectionMatrix)
	{
		updateProjectionMatrix();
	}

	return projectionMatrix;
}

mat4 const& PointLight::getViewMatrix() const
{
	if (!validViewMatrix)
	{
		updateViewMatrix();
	}

	return viewMatrix;
}

GLuint PointLight::createShadowTexture(GLsizei size, GLuint& framebuffer)
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
	: validProjectionMatrix(false), validViewMatrix(false), validViewProjectionMatrix(false), target(1, 0, 0, 0)
{
	shadowResolution = shadowSize;
	shadowTexture = createShadowTexture(shadowResolution, shadowBuffer);
}

PointLight::PointLight(vec4 const& position, vec4 const& target, vec3 const& intensity)
	: position(position), intensity(intensity), target(target),
	 validProjectionMatrix(false), validViewMatrix(false), validViewProjectionMatrix(false)
{
	shadowResolution = shadowSize;
	shadowTexture = createShadowTexture(shadowResolution, shadowBuffer);
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
	validProjectionMatrix = false;
	validViewProjectionMatrix = false;
}

void PointLight::setTarget(vec4 const& target)
{
	this->target = target;

	validViewMatrix = false;
	validProjectionMatrix = false;
	validViewProjectionMatrix = false;
}

void PointLight::setIntensity(vec3 const& intestity)
{
	this->intensity = intestity;
}

void PointLight::setPoints(std::vector<glm::vec3> const& points)
{
	this->points = points;

	validProjectionMatrix = false;
	validViewProjectionMatrix = false;
}

vec4 const& PointLight::getPosition() const
{
	return position;
}

vec4 const& PointLight::getTarget() const
{
	return target;
}

vec3 const& PointLight::getIntensity() const
{
	return intensity;
}

std::vector<glm::vec3> const& PointLight::getPoints() const
{
	return points;
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

		validViewProjectionMatrix = true;
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