#pragma once

#include "Screen.h"
#include "Model.h"

class GameScreen :
	public Screen
{
private:
	std::vector<PointLight::ptr> primaryLights;

public:
	Model::ptr test[100];

	GameScreen();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();
};