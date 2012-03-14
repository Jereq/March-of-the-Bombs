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

/**
 * Model represents a single instance of a model in the scene.
 */
class Model
{
protected:
	ModelData::ptr modelData;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	bool selected;
	glm::vec4 tint;

	mutable bool validModelMatrix;
	mutable glm::mat4 modelMatrix;

	mutable bool validInverseModelMatrix;
	mutable glm::mat4 inverseModelMatrix;

public:
	typedef boost::shared_ptr<Model> ptr;

	/// Constructor.
	Model(ModelData::ptr const& model);
	/// Destructor.
	virtual ~Model();

	/// Add the model to the model data's list of instances to draw
	virtual void addInstance();

	/// Change the current position of the model
	void setPosition(glm::vec3 const& position);
	/// Change the current rotation of the model
	void setRotation(glm::vec3 const& rotation);
	/// Change the current scale of the model
	void setScale(glm::vec3 const& scale);

	/// Get the current position of the model
	glm::vec3 const& getPosition() const;
	/// Get the current rotation of the model
	glm::vec3 const& getRotation() const;
	/// Get the current scale of the model
	glm::vec3 const& getScale() const;

	/// Get the model matrix for the model
	glm::mat4 const& getModelMatrix() const;
	/// Get the inverse of the model matrix
	glm::mat4 const& getInverseModelMatrix() const;

	/// Set the models selection state
	void setSelected(bool select);
	/// Get the models selection state
	bool isSelected() const;

	/// Set the tint of the model
	void setTint(glm::vec4 const& tint);
	/// Get the current tint of the model
	glm::vec4 const& getTint() const;

	/**
	 * Intersect the models bounding box with a ray. Return true if the ray
	 * intersect the bounding box closer than distance. If true, also set distance to the
	 * intersection deistance.
	 */
	virtual bool rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const;
	/**
	 * Intersect the models bounding box with a frustum. Return true if the bounding box
	 * is inside the frustum, otherwise false.
	 */
	virtual bool frustumIntersect(Frustum const& frustum) const;
};