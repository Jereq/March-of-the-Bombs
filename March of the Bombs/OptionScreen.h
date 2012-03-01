#pragma once

#include "Screen.h"
#include "KeyboardEvent.h"
#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"

class Game;

class OptionScreen
{
private:
	std::vector<Button> buttons;
	std::vector<SimpleImage> Backgrounds;
	Screen::ptr nextScreen;
	boost::shared_ptr<Game> game;

public:
	OptionScreen();
	~OptionScreen();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void createButtons();
	void createBackground();
	void KeyboardEventMethod(KeyboardEvent* keyEvent);
	void MousePressEventMethod(MouseButtonEvent* mbEvent);
	void MouseTouchEventMethod(MouseMoveEvent* mmEvent);
};

