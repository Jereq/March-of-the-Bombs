#pragma once

#include <list>
#include <map>
#include <set>

#include "Bomb.h"
#include "Explosion.h"
#include "Flag.h"
#include "Game.h"
#include "HQBlock.h"
#include "LoudEntity.h"
#include "Map.h"
#include "Screen.h"

#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "MouseMoveEvent.h"

class GameScreen :
	public Screen
{
private:
	Game::ptr game;
	AttachmentPoint::ptr cameraPos;
	float cameraSpeed;
	float cameraRotationSpeed;
	Map blockMap;

	std::vector<Block::ptr> flags;
	std::vector<boost::shared_ptr<LoudEntity>> loudEntities;
	boost::shared_ptr<LoudEntity> zombie;

	unsigned short myEntityCount;

	typedef std::map<unsigned short, Bomb> entity_map;
	entity_map myEntities;

	std::vector<Button> buttons;
	Screen::ptr nextScreen;
	TextureSection::ptr background;
	SimpleImage::ptr GuiBackground;

	float currentDeltaTime;
	glm::vec3 cameraVelocity;
	float rotationYSpeed;
	float rotationXSpeed;
	PointLight::ptr light;

	std::list<Explosion> explosions;
	int explosionsThisFrame;

	std::set<TextureSection::ptr> preloadedTextures;

	void spawnBomb(glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity);
	void createExplosion(glm::vec3 const& position, float size, float duration, bool removeBlocks);

	void preloadTextures();

	void createButtons();
	void createBackground();

	void selectBombsBox(glm::vec2 const& pos1, glm::vec2 const& pos2);
	void selectBombRay(glm::vec2 const& pos);

	void getNearbyBombs(glm::vec2 const& center, float distance, Bomb::id_set& res) const;

	void keyboardEventHandler(KeyboardEvent const* kbEvent);
	void mouseButtonEventHandler(MouseButtonEvent const* mbEvent);
	void mouseMoveEventHandler(MouseMoveEvent const* mmEvent);

public:
	GameScreen();

	void atEntry();
	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();
};