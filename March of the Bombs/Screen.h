#pragma once

#include <boost\shared_ptr.hpp>
#include "Graphics.h"

class Screen
{

public:
	virtual void update() = 0;
	virtual void draw(Graphics::ptr graphics) = 0;
	typedef boost::shared_ptr<Screen> ptr;

};