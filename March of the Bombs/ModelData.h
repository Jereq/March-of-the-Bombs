#pragma once

#include <boost/shared_ptr.hpp>

#include "GLSLProgram.h"
#include "BoundingBox.h"

class ModelData
{
public:
	typedef boost::shared_ptr<ModelData> ptr;

	virtual ~ModelData() {}

	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix, bool selected) = 0;
	virtual void clearInstancesToDraw() = 0;
	
	virtual void drawInstances(GLSLProgram const& prog) const = 0;
	virtual void drawInstancesShadow(GLSLProgram const& prog) const = 0;

	virtual BoundingBox getBoundingBox() const = 0;
};