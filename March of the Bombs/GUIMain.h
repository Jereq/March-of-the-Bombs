#pragma once

#include "Screen.h"
#include "Graphics.h"

class GUIMain
{
private:
	Screen::ptr screen;						//smartpointer pointing at screen

public:
	typedef boost::shared_ptr<GUIMain> ptr;

	GUIMain();								//constructor
	virtual ~GUIMain();						//destructor
	void update(float deltaTime);			//updatefunction
	void draw(Graphics::ptr graphics);		//drawfunction
};