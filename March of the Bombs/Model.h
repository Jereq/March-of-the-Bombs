#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "Model3DS.h"
#include "GLSLProgram.h"
#include "PointLight.h"

class Graphics;

class Model
{
protected:
	static std::map<std::string, Model3DS::ptr> modelMap;

	Model3DS::ptr modelData;
	static GLSLProgram shadeProg;
	static GLSLProgram shadowProg;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	mutable bool validModelMatrix;
	mutable glm::mat4 modelMatrix;

	void loadShadeProg();
	void loadShadowProg();

public:
	typedef boost::shared_ptr<Model> ptr;

	Model(std::string const& fileName);
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