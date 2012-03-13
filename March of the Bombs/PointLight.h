#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <boost/shared_ptr.hpp>

class PointLight
{
private:
	glm::vec4 position;
	glm::vec4 target;
	glm::vec3 intensity;
	std::vector<glm::vec3> points;

	mutable bool validProjectionMatrix;
	mutable glm::mat4 projectionMatrix;

	mutable bool validViewMatrix;
	mutable glm::mat4 viewMatrix;

	mutable bool validViewProjectionMatrix;
	mutable glm::mat4 viewProjectionMatrix;

	GLuint shadowTexture;
	GLuint shadowBuffer;
	GLsizei shadowResolution;

	GLuint createShadowTexture(GLsizei size, GLuint& framebuffer);

	void updateViewMatrix() const;
	void updateProjectionMatrix() const;
	
public:
	typedef boost::shared_ptr<PointLight> ptr;

	PointLight();
	PointLight(glm::vec4 const& position, glm::vec4 const& target, glm::vec3 const& intensity);
	virtual ~PointLight();

	void setPosition(glm::vec4 const& position);
	void setTarget(glm::vec4 const& target);
	void setIntensity(glm::vec3 const& intesity);
	void setPoints(std::vector<glm::vec3> const& points);

	glm::vec4 const& getPosition() const;
	glm::vec4 const& getTarget() const;
	glm::vec3 const& getIntensity() const;
	std::vector<glm::vec3> const& getPoints() const;

	glm::mat4 const& getProjectionMatrix() const;
	glm::mat4 const& getViewMatrix() const;
	glm::mat4 const& getViewProjectionMatrix() const;

	GLuint getShadowTexture() const;
	GLuint getShadowBuffer() const;
	GLsizei getShadowResolution() const;
};