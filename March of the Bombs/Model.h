#pragma once

#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "ModelData.h"
#include "GLSLProgram.h"
#include "GLTexture.h"
#include "PointLight.h"

class Graphics;

class Model
{
protected:
	ModelData::ptr modelData;
	std::vector<GLTexture::ptr> textures;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	bool selected;
	glm::vec4 tint;

	mutable bool validModelMatrix;
	mutable glm::mat4 modelMatrix;

	mutable bool validInverseModelMatrix;
	mutable glm::mat4 inverseModelMatrix;

	void loadShadeProg();
	void loadShadowProg();

public:
	typedef boost::shared_ptr<Model> ptr;

	Model(ModelData::ptr const& model);
	virtual ~Model();

	virtual void addInstance();

	void setPosition(glm::vec3 const& position);
	void setRotation(glm::vec3 const& rotation);
	void setScale(glm::vec3 const& scale);

	glm::vec3 const& getPosition() const;
	glm::vec3 const& getRotation() const;
	glm::vec3 const& getScale() const;

	glm::mat4 const& getModelMatrix() const;
	glm::mat4 const& getInverseModelMatrix() const;

	void setSelected(bool select);
	bool isSelected() const;

	void setTint(glm::vec4 const& tint);
	glm::vec4 const& getTint() const;

	virtual bool rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const;
};