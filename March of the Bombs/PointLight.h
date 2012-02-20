#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <boost/shared_ptr.hpp>

class PointLight
{
private:
	glm::vec4 position;
	glm::vec3 intensity;

	mutable bool validProjectionMatrix;
	mutable glm::mat4 projectionMatrix;

	mutable bool validViewMatrix;
	mutable glm::mat4 viewMatrix;

	mutable bool validViewProjectionMatrix;
	mutable glm::mat4 viewProjectionMatrix;

	GLuint shadowTexture;
	GLuint shadowBuffer;


	GLuint getShadowTexture(GLsizei width, GLsizei height, GLuint& framebuffer);
	
public:
	typedef boost::shared_ptr<PointLight> ptr;

	PointLight();
	PointLight(glm::vec4 position, glm::vec3 intensity);
	virtual ~PointLight();

	void setPosition(glm::vec4 const& position);
	void setIntensity(glm::vec3 const& intesity);

	glm::vec4 const& getPosition() const;
	glm::vec3 const& getIntensity() const;

	glm::mat4 const& getProjectionMatrix() const;
	glm::mat4 const& getViewMatrix() const;
	glm::mat4 const& getViewProjectionMatrix() const;

	GLuint getShadowTexture() const;
	GLuint getShadowBuffer() const;
};