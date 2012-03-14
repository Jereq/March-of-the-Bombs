#pragma once

#include <boost/shared_ptr.hpp>

#include "GLSLProgram.h"
#include "BoundingBox.h"

/**
 * ModelData is a abstract base class for an object that holds static model data,
 * that is shared between instances of that same model type.
 */
class ModelData
{
public:
	typedef boost::shared_ptr<ModelData> ptr;

	/// Destructor.
	virtual ~ModelData() {}

	/**
	 * Add a model instance of a derived class to a list of instances. As of yet,
	 * the only information that may change between instances is the model matrix and a tint.
	 */
	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint) = 0;
	/// Clear the list of instances that are to be drawn
	virtual void clearInstancesToDraw() = 0;
	
	/// Draw instances normally
	virtual void drawInstances(GLSLProgram const& prog) const = 0;
	/// Draw instances from a light source onto a shadow map
	virtual void drawInstancesShadow(GLSLProgram const& prog) const = 0;

	/// Get the model base's bounding box, unaffected by any model matrix
	virtual BoundingBox getBoundingBox() const = 0;
};