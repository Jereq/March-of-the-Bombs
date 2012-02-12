#pragma once

#include <boost\shared_ptr.hpp>							//include a boost smartpointer
#include "Graphics.h"
#include "Button.h"
#include "GLTexture.h"
#include "TGALoader.h"
#include <vector>

class Screen
{

public:
	virtual void update() = 0;							//Makes the other classes aware of the funktion, here they are abstract and not used
	virtual void draw(Graphics::ptr graphics) = 0;		//Same as fore update-funktion
	typedef boost::shared_ptr<Screen> ptr;				//A smartpointer
	
};