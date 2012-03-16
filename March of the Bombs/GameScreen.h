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
#include "HQBlock.h"

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

	std::string myName;
	std::string opponentName;

	unsigned short myID;
	unsigned short opponentID;

	glm::vec3 myColor;
	glm::vec3 opponentColor;

	float myScore;
	float opponentScore;

	glm::ivec2 basePosition;
	glm::vec3 opponentBasePos;

	unsigned short myEntityCount;

	typedef std::map<unsigned short, Bomb> entity_map;
	entity_map myEntities;
	entity_map opponentEntities;

	const static float TIME_PER_BOMB;
	float timeToNextBomb;

	std::vector<Button> buttons;
	std::vector<SimpleImage> Backgrounds;
	Screen::ptr nextScreen;
	TextureSection::ptr background;

	float currentDeltaTime;
	glm::vec3 cameraVelocity;
	float rotationYSpeed;
	float rotationXSpeed;
	PointLight::ptr light;

	glm::vec2 selectionPosition;
	bool selecting;

	const static float BASE_POINTS_PER_BOMB;
	const static float FLAG_POINTS_PER_SEC;
	const static float FLAG_RADIUS;
	const static float EXPLOSION_RADIUS;
	std::list<Explosion> explosions;
	int explosionsThisFrame;

	std::set<TextureSection::ptr> preloadedTextures;

	void spawnBomb(glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity);
	void createExplosion(glm::vec3 const& position, float size, float duration, bool removeBlocks);

	void preloadTextures();
	void selectBombsBox(glm::vec2 const& pos1, glm::vec2 const& pos2);
	void selectBombRay(glm::vec2 const& pos);

	void getNearbyBombs(glm::vec2 const& center, float distance, Bomb::id_set& res) const;

public:
	GameScreen(GameClient::ptr const& client, std::string const& myName, unsigned short myID, glm::vec3 const& myColor,
		std::string const& opponentName, unsigned short opponentID, glm::vec3 const& opponentColor,
		std::string const& mapName, unsigned short myBaseID);

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
	void handlePacket15UpdatePlayerScore(Packet15UpdatePlayerScore::const_ptr const& packet);
};