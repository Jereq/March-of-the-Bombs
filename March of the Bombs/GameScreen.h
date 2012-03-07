#pragma once

#include <list>
#include <map>

#include "Screen.h"
#include "Game.h"
#include "MapHeader.h"
#include "Bomb.h"

#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "MouseMoveEvent.h"

class GameScreen :
	public Screen
{
private:
	Game::ptr game;
	GameClient::ptr client;
	AttachmentPoint::ptr cameraPos;
	Map blockMap;

	unsigned short myID;
	unsigned short opponentID;

	unsigned short myEntityCount;

	typedef std::map<unsigned short, Bomb> entity_map;
	entity_map myEntities;
	entity_map opponentEntities;

	std::vector<Button> buttons;
	std::vector<SimpleImage> Backgrounds;
	Screen::ptr nextScreen;

	float currentDeltaTime;
	glm::vec3 cameraVelocity;
	float rotationYSpeed;
	float rotationXSpeed;
	PointLight::ptr light;

public:
	GameScreen();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void createButtons();
	void createBackground();
	void keyboardEventHandler(KeyboardEvent const* kbEvent);
	void mouseButtonEventHandler(MouseButtonEvent const* mbEvent);
	void mouseMoveEventHandler(MouseMoveEvent const* mmEvent);
	
	virtual void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet);
};