#pragma once

#include "Screen.h"

class GUIMain
{
private:
	Screen::ptr screen;

public:
	GUIMain();
	virtual ~GUIMain();
	void update();
	void draw();
};

