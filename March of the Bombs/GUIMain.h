#pragma once

#include "Screen.h"
#include "Graphics.h"

class GUIMain
{
private:
	Screen::ptr screen;						//smartpointer pointing at screen

public:
	GUIMain();								//constructor
	virtual ~GUIMain();						//destructor
	void update(float deltaTime);			//updatefunction
	void draw(Graphics::ptr graphics);		//drawfunction
};

