#pragma once

#include "Screen.h"
#include "Model.h"

class GameScreen :
	public Screen
{
private:
	std::vector<PointLight::ptr> primaryLights;

public:
	Model::ptr test;

	GameScreen();

	void update();
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();
};