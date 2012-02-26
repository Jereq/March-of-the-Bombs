#pragma once

#include <boost\shared_ptr.hpp>							//include a boost smartpointer
#include "Graphics.h"
#include "Button.h"
#include "SimpleImage.h"
#include "GLTexture.h"
#include <vector>

class Screen
{

public:
	typedef boost::shared_ptr<Screen> ptr;				//A smartpointer

	virtual void update(float deltaTime) = 0;			//Makes the other classes aware of the function, here they are abstract and not used
	virtual void draw(Graphics::ptr graphics) = 0;		//Same as for update-function
	virtual ptr getNextScreen() = 0;
};