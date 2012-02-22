#pragma once

#include "Screen.h"

class MainMeny :
	public Screen
{

private:
	std::vector<Button> buttons;
	std::vector<SimpleImage> Backgrounds;
	Screen::ptr nextScreen;

public:
	MainMeny(void);
	~MainMeny(void);
	void update();
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void createButtons();
	void createBackground();
};