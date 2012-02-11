#pragma once

#include "Screen.h"

class MainMeny :
	public Screen
{

public:
	MainMeny(void);
	~MainMeny(void);
	void update();
	void draw(Graphics::ptr graphics);
};

