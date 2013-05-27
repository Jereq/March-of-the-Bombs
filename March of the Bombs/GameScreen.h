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

/*
 * GameScreen is the main class for the actual game,
 * containing the majority of the game logic.
 */
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

	std::vector<Button> buttons;
	Screen::ptr nextScreen;
	TextureSection::ptr background;
	SimpleImage::ptr GuiBackground;

	glm::vec3 cameraVelocity;
	float rotationYSpeed;
	float rotationXSpeed;
	PointLight::ptr light;

	void addEntities();

	void createButtons();
	void createBackground();

	void handleEvents();
	void keyboardEventHandler(KeyboardEvent const* kbEvent);
	void mouseButtonEventHandler(MouseButtonEvent const* mbEvent);
	void mouseMoveEventHandler(MouseMoveEvent const* mmEvent);

	void updateZombie(float deltaTime);
	void updateCamera(float deltaTime);
	void updateFilter();

public:
	/*
	 * constructor.
	 */
	GameScreen();

	void atEntry() override;
	void update(float deltaTime) override;
	void draw(Graphics::ptr graphics) override;
	Screen::ptr getNextScreen() override;
};