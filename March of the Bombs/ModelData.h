#pragma once

#include <boost/shared_ptr.hpp>

#include "GLSLProgram.h"

class ModelData
{
public:
	typedef boost::shared_ptr<ModelData> ptr;

	virtual ~ModelData() {}
	
	virtual void draw(GLSLProgram const& prog) const = 0;
	virtual void drawShadow() const = 0;
};