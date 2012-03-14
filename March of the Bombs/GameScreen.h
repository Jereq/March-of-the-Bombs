#pragma once

#include <list>
#include <map>
#include <set>

#include "Screen.h"
#include "Game.h"
#include "Map.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Flag.h"

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
	float cameraSpeed;
	float cameraRotationSpeed;
	Map blockMap;

	unsigned short myID;
	unsigned short opponentID;
	glm::vec3 opponentColor;
	glm::ivec2 basePosition;

	unsigned short myEntityCount;

	typedef std::map<unsigned short, Bomb> entity_map;
	entity_map myEntities;
	entity_map opponentEntities;
	Flag flag;	

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

	glm::vec2 selectionPosition;
	bool selecting;

	std::list<Explosion> explosions;

	std::set<TextureSection::ptr> preloadedTextures;

	void spawnBomb(glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity);

	void preloadTextures();
	void selectBombsBox(glm::vec2 const& pos1, glm::vec2 const& pos2);
	void selectBombRay(glm::vec2 const& pos);

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
	void handlePacket13RemoveBomb(Packet13RemoveBomb::const_ptr const& packet);
	void handlePacket14RemoveBlocks(Packet14RemoveBlocks::const_ptr const& packet);
};