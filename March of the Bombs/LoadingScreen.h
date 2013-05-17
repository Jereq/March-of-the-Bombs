#pragma once

#include <boost/thread.hpp>

#include "Screen.h"
#include "GameScreen.h"
#include "Label.h"

class LoadingScreen
	: public Screen
{
private:
	boost::thread loadThread;

	Screen::ptr newGame;
	Screen::ptr nextScreen;

	void loadGame();

public:
	typedef boost::shared_ptr<LoadingScreen> ptr;

	LoadingScreen();

	void atEntry();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();
};