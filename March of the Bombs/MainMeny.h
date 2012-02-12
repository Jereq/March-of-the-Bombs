#pragma once

#include "Screen.h"


class MainMeny :
	public Screen
{

private:
	std::vector<Button> buttons;
	std::vector<SimpleImage> Backgrounds;
	

public:
	MainMeny(void);
	~MainMeny(void);
	void update();
	void draw(Graphics::ptr graphics);
	void createButtons();
	void createBackground();
};

