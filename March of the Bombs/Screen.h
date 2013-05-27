#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/foreach.hpp>

#include "Graphics.h"
#include "Button.h"
#include "SimpleImage.h"

class Screen
	: public boost::enable_shared_from_this<Screen>
{
public:
	typedef boost::shared_ptr<Screen> ptr;

	/*
	 * Called after the screen has been created and before it is updated or drawn.
	 */
	virtual void atEntry() {}

	virtual void update(float deltaTime) =0;			//Makes the other classes aware of the function, here they are abstract and not used
	virtual void draw(Graphics::ptr graphics) =0;		//Same as for update-function
	virtual ptr getNextScreen() =0;
};