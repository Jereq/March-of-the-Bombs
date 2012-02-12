#pragma once

#include "Screen.h"

class MainMeny :
	public Screen
{

private:
	std::vector<Button> buttons;

public:
	MainMeny(void);
	~MainMeny(void);
	void update();
	void draw(Graphics::ptr graphics);

};

