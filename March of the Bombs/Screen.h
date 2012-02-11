#pragma once

#include <boost\shared_ptr.hpp>

class Screen
{

public:
	virtual void update() = 0;
	virtual void draw() = 0;
	typedef boost::shared_ptr<Screen> ptr;

};