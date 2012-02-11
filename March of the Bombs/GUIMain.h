#pragma once

#include "Screen.h"
#include "Graphics.h"

class GUIMain
{
private:
	Screen::ptr screen;

public:
	GUIMain();
	virtual ~GUIMain();
	void update();
	void draw(Graphics::ptr graphics);
};

