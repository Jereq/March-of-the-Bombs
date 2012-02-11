#pragma once

#include <boost/shared_ptr.hpp>

#include "GLTexture.h"

class Graphics
{
private:
public:
	typedef boost::shared_ptr<Graphics> ptr;

	void drawTexture(GLTexture::ptr texture);
};