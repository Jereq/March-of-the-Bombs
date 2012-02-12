#pragma once

#include <boost/shared_ptr.hpp>

#include "GLTexture.h"
#include "GLSLProgram.h"

class Graphics
{
private:
	GLSLProgram prog;

	void loadShaders();

public:
	typedef boost::shared_ptr<Graphics> ptr;

	Graphics();

	void drawTexture(GLTexture::ptr texture);
};