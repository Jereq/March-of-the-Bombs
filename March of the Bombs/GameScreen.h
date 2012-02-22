#pragma once

#include "Screen.h"

class GameScreen :
	public Screen
{
public:
	GameScreen();

	void update();
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();
};