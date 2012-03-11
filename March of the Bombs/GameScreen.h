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
	glm::vec3 opponentColor;
	glm::ivec2 basePosition;

	unsigned short myEntityCount;

	typedef std::map<unsigned short, Bomb> entity_map;
	entity_map myEntities;
	entity_map opponentEntities;

	const static float TIME_PER_BOMB;
	float timeToNextBomb;

	std::vector<Button> buttons;
	std::vector<SimpleImage> Backgrounds;
	Screen::ptr nextScreen;

	float currentDeltaTime;
	glm::vec3 cameraVelocity;
	float rotationYSpeed;
	float rotationXSpeed;
	PointLight::ptr light;

	void spawnBomb(glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity);

public:
	GameScreen(GameClient::ptr const& client, std::string const& mapName, unsigned short myID,
		unsigned short opponentID, unsigned short myBaseID, glm::vec3 const& opponentColor);

	void atEntry();
	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void createButtons();
	void createBackground();
	void keyboardEventHandler(KeyboardEvent const* kbEvent);
	void mouseButtonEventHandler(MouseButtonEvent const* mbEvent);
	void mouseMoveEventHandler(MouseMoveEvent const* mmEvent);
	
	void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet);
	void handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet);
};