#pragma once

#include <glm/glm.hpp>

#include "Model3DS.h"
#include "GLSLProgram.h"
#include "Graphics.h"

class Model
{
protected:
	Model3DS::ptr modelData;
	GLSLProgram shadeProg;
	GLSLProgram shadowProg;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	mutable bool validModelMatrix;
	mutable glm::mat4 modelMatrix;

	Model(Model3DS::ptr modelData);

	void loadShadeProg();
	void loadShadowProg();

public:
	virtual ~Model();

	virtual void draw(Graphics const& graphics) const;
	virtual void drawShadow(PointLight::ptr const& light) const;

	void setPosition(glm::vec3 const& position);
	void setRotation(glm::vec3 const& rotation);
	void setScale(glm::vec3 const& scale);

	glm::vec3 const& getPosition() const;
	glm::vec3 const& getRotation() const;
	glm::vec3 const& getScale() const;

	glm::mat4 const& getModelMatrix() const;
};