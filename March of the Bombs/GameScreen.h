#pragma once

#include "Screen.h"
#include "Model.h"
#include "Game.h"
#include "MapHeader.h"

#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "MouseMoveEvent.h"

class GameScreen :
	public Screen
{
private:
	Game::ptr game;
	AttachmentPoint::ptr cameraPos;
	Map blockMap;

	float currentDeltaTime;
	glm::vec3 cameraVelocity;
	float rotationYSpeed;
	float rotationXSpeed;

	Model::ptr groundPlane;
	std::vector<PointLight::ptr> primaryLights;

public:
	const static int testCount = 50;
	Model::ptr test[testCount];
	std::list<glm::vec3> testPath[testCount];

	GameScreen();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void keyboardEventHandler(KeyboardEvent const* kbEvent);
	void mouseButtonEventHandler(MouseButtonEvent const* mbEvent);
	void mouseMoveEventHandler(MouseMoveEvent const* mmEvent);
};