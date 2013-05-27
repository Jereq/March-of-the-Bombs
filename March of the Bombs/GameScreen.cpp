#include "GameScreen.h"

#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include "Game.h"
#include "MainMeny.h"
#include "BlockModelData.h"
#include "PlaneModelData.h"
#include "StandardBombModelData.h"
#include "FlagModelData.h"
#include "HQModelData.h"

GameScreen::GameScreen()
	: game(Game::getInstance()),
	  rotationYSpeed(0), rotationXSpeed(0), blockMap("Maps/emptyplane 75x75.txt"),
	  cameraSpeed(5.67f), cameraRotationSpeed(180.f)
{
	std::vector<glm::ivec2> const& bases = blockMap.getBases();
	assert(bases.size() > 0);	// Maps without bases is incorrect
	glm::ivec2 basePosition = bases[0];

	// Place camera next to first base
	cameraPos.reset(new AttachmentPoint(glm::vec3(basePosition.x, 1.8f, basePosition.y + 2.f), glm::vec3(-10.f, 180.f, 0.f)));

	// Place flags
	std::vector<glm::ivec2> const& flagPos = blockMap.getFlags();
	for (glm::ivec2 const& pos : flagPos)
	{
		flags.push_back(blockMap.getBlock(pos));
	}
}

void GameScreen::addEntities()
{
	// Place loud entities with different sounds

	boost::shared_ptr<LoudEntity> entity;

	entity.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/fire-1.mp3"));
	entity->setPosition(glm::vec3(38.f, 0.f, 33.f));
	loudEntities.push_back(boost::shared_ptr<LoudEntity>(entity));

	entity.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/20043.mp3"));
	entity->setPosition(glm::vec3(60.f, 0.f, 22.f));
	loudEntities.push_back(boost::shared_ptr<LoudEntity>(entity));

	entity.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/Cartoon Voice Baritone-SoundBible.com-2068310080.mp3"));
	entity->setPosition(glm::vec3(58.f, 0.f, 54.f));
	loudEntities.push_back(boost::shared_ptr<LoudEntity>(entity));

	entity.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/12376.mp3"));
	entity->setPosition(glm::vec3(15.f, 0.f, 15.f));
	loudEntities.push_back(boost::shared_ptr<LoudEntity>(entity));

	zombie.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/Zombie Gibberish-SoundBible.com-589887278.mp3"));
	zombie->setVolume(0.5f);	// Zombie is loud :(

	// Place "zombie" bomb at opponents base
	glm::vec3 zombiePos;
	auto bases = blockMap.getBases();
	if (bases.size() > 1)
	{
		zombiePos = glm::vec3(bases[1].x, 0.f, bases[1].y);
	}
	else
	{
		zombiePos = cameraPos->getPosition();
		zombiePos.y = 0.f;
	}
	zombie->setPosition(zombiePos);
	loudEntities.push_back(boost::shared_ptr<LoudEntity>(zombie));
}

void GameScreen::atEntry()
{
	createBackground();
	createButtons();

	// Attach camera to the players position
	Graphics::ptr graphics = game->getGraphics();
	graphics->getCamera()->setAttachmentPoint(cameraPos);

	glm::ivec2 mapSize = blockMap.getSize();

	light = PointLight::ptr(
		new PointLight(
			glm::vec4(mapSize.x / 2.f, 70, mapSize.y / 2.f, 1),
			glm::vec4(0, -1, 0, 0),
			glm::vec3(4.f)));

	// Calculate the bounds of the map for shadowmapping
	std::vector<glm::vec3> mapPoints;
	mapPoints.push_back(glm::vec3(      0.f, 0.f,       0.f));
	mapPoints.push_back(glm::vec3(      0.f, 3.f,       0.f));
	mapPoints.push_back(glm::vec3(      0.f, 0.f, mapSize.y));
	mapPoints.push_back(glm::vec3(      0.f, 3.f, mapSize.y));
	mapPoints.push_back(glm::vec3(mapSize.x, 0.f,       0.f));
	mapPoints.push_back(glm::vec3(mapSize.x, 3.f,       0.f));
	mapPoints.push_back(glm::vec3(mapSize.x, 0.f, mapSize.y));
	mapPoints.push_back(glm::vec3(mapSize.x, 3.f, mapSize.y));
	light->setPoints(mapPoints);

	// Set the games single light
	graphics->setLight(light);

	// Set background music and background image
	game->getSoundManager()->playBackgroundSound("Sounds/gamebackground.mp3", 0.5f);
	game->getGraphics()->setBackground(background);

	// Add the entities with their sounds
	addEntities();
}

void GameScreen::handleEvents()
{
	// Get the current events
	std::deque<Event::ptr>& events = game->getEvents();
	std::deque<Event::ptr>::size_type numEvents = events.size();
	std::deque<Event::ptr>::size_type eventsHandled = 0;

	// Handle a max of numEvents events to prevent infinite loop
	while (!events.empty() && eventsHandled++ < numEvents)
	{
		// Get first event
		Event::ptr ev = events.front();
		events.pop_front();

		// Dispatch event
		switch (ev->eventType)
		{
		case EventType::Keyboard:
			{
				KeyboardEvent* keyEvent = static_cast<KeyboardEvent*>(ev.get());
				keyboardEventHandler(keyEvent);
			}
			break;

		case EventType::MouseButton:
			{
				MouseButtonEvent* mbEvent = static_cast<MouseButtonEvent*>(ev.get());				
				mouseButtonEventHandler(mbEvent);
				
			}
			break;
		case EventType::MouseMove:
			{
				MouseMoveEvent* mmEvent = static_cast<MouseMoveEvent*>(ev.get());
				mouseMoveEventHandler(mmEvent);
			}
			break;
		}
	}
}

void GameScreen::updateZombie(float deltaTime)
{
	// Get player ground position
	glm::vec3 playerPos(cameraPos->getPosition());
	playerPos.y = 0.f;
	glm::vec3 zombieOffset(playerPos - zombie->getPosition());

	const static float ZOMBIE_SPEED = 1.3f;

	// Check if the zombie reaches the player this update
	if (glm::length(zombieOffset) > ZOMBIE_SPEED * deltaTime)
	{
		// Move towards the player
		glm::vec3 direction = glm::normalize(zombieOffset);
		zombie->setPosition(zombie->getPosition() + direction * ZOMBIE_SPEED * deltaTime);
	}
	else
	{
		// Move to player
		zombie->setPosition(playerPos);
	}

	// If was not at player, turn toward the player
	if (zombieOffset != glm::vec3())
	{
		zombie->setDirection(zombieOffset);
	}
}

void GameScreen::updateCamera(float deltaTime)
{
	// Get current rotation and add turning to it
	glm::vec3 rotation = cameraPos->getRotation();
	rotation.y += rotationYSpeed * deltaTime;
	rotation.x += rotationXSpeed * deltaTime;

	// Bound up/down rotation, prevents falling on your back
	if (rotation.x > 35.f)
	{
		rotation.x = 35.f;
	}
	else if (rotation.x < -90)
	{
		rotation.x = -90;
	}

	// Set new rotation
	cameraPos->setRotation(rotation);

	// Move the camera in the ground direction
	glm::mat3 rotationMatrix = glm::mat3(glm::rotate(glm::mat4(), rotation.y, glm::vec3(0, 1, 0)));
	cameraPos->setPosition(cameraPos->getPosition() + rotationMatrix * cameraVelocity * deltaTime);
}

void GameScreen::updateFilter()
{
	// Get player position
	glm::vec3 playerPos(cameraPos->getPosition());

	// If in first room
	if (playerPos.x >= 0.f && playerPos.x <= 13.f
		&& playerPos.z >= 0.f && playerPos.z <= 12.f)
	{
		game->getSoundManager()->setFilterType(SoundManager::FilterType::ECHO);
	}
	// If in second room
	else if (playerPos.x >= 0.f && playerPos.x <= 13.f
		&& playerPos.z > 12.f && playerPos.z <= 23.f)
	{
		game->getSoundManager()->setFilterType(SoundManager::FilterType::DAMPENING);
	}
	// Outside
	else
	{
		game->getSoundManager()->setFilterType(SoundManager::FilterType::NORMAL);
	}
}

void GameScreen::update(float deltaTime)
{
	// Skip if we already has decided to stop
	if (nextScreen)
	{
		return;
	}

	// Handle any window events since last update
	handleEvents();

	updateZombie(deltaTime);
	updateCamera(deltaTime);
	updateFilter();
}

void GameScreen::draw(Graphics::ptr graphics)
{
	for (auto& entity : loudEntities)
	{
		entity->draw(graphics);
	}

	// Draw terrain
	blockMap.draw(graphics);

	// Starts to render all the backgrounds
	GuiBackground->render(graphics);

	// Starts to render all the buttons
	for (Button& button : buttons)
	{
		button.render(graphics);
	}
}

Screen::ptr GameScreen::getNextScreen()
{
	// Move the next screen to the caller
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void GameScreen::keyboardEventHandler(KeyboardEvent const* kbEvent)
{
	if (kbEvent->keyEventType == KeyboardEventType::Pressed)
	{
		const static char ESC = 0x1B;

		switch (kbEvent->key)
		{
		case ESC:
			game->close();
			break;

		case 'a':
			cameraVelocity.x += -cameraSpeed;
			break;

		case 'd':
			cameraVelocity.x += cameraSpeed;
			break;

		case 'w':
			cameraVelocity.z += -cameraSpeed;
			break;

		case 's':
			cameraVelocity.z += cameraSpeed;
			break;

		case 'q':
			rotationYSpeed += cameraRotationSpeed;
			break;

		case 'e':
			rotationYSpeed += -cameraRotationSpeed;
			break;

		case 'r':
			rotationXSpeed += cameraRotationSpeed;
			break;

		case 'f':
			rotationXSpeed += -cameraRotationSpeed;
			break;

		case '+':
			cameraRotationSpeed += 5;
			break;

		case '-':
			cameraRotationSpeed -= 5;
			break;
		}
	}

	if (kbEvent->keyEventType == KeyboardEventType::Released)
	{
		switch (kbEvent->key)
		{
		case 'a':
			cameraVelocity.x -= -cameraSpeed;
			break;

		case 'd':
			cameraVelocity.x -= cameraSpeed;
			break;

		case 'w':
			cameraVelocity.z -= -cameraSpeed;
			break;

		case 's':
			cameraVelocity.z -= cameraSpeed;
			break;

		case 'q':
			rotationYSpeed -= cameraRotationSpeed;
			break;

		case 'e':
			rotationYSpeed -= -cameraRotationSpeed;
			break;

		case 'r':
			rotationXSpeed -= cameraRotationSpeed;
			break;

		case 'f':
			rotationXSpeed -= -cameraRotationSpeed;
			break;
		}
	}
}

void GameScreen::mouseButtonEventHandler(MouseButtonEvent const* mbEvent)
{
	if (mbEvent->state == MouseButtonState::Pressed)
	{
		switch (mbEvent->button)
		{
		case MouseButton::Left:
			if (buttons[0].getState() == Hovered)
			{
				// Switch screen
				nextScreen = Screen::ptr(new MainMeny());
				game->getEvents().clear();
			}
			break;
		}
	}
}

void GameScreen::mouseMoveEventHandler(MouseMoveEvent const* mmEvent)
{
	for (Button& button : buttons)
	{
		if(button.intersects(mmEvent->position))
		{
			button.setState(Hovered);
		}
		else
		{
			button.setState(Unused);
		}
	}
}

void GameScreen::createBackground()
{
	background.reset(new TextureSection(L"Images/CaveBackground.png"));
	TextureSection guiBackground(L"Images/GuiBackground.png");

	GuiBackground = SimpleImage::ptr(new SimpleImage(guiBackground, Rectanglef(glm::vec2(0.00f,0.92f),glm::vec2(1.00f,0.08f)), 0.10f, glm::vec3(1)));
}

void GameScreen::createButtons()
{
	TextureSection BackButton(L"Images/NewBI/ExitBtn1.png");
	TextureSection BackButtonT(L"Images/NewBI/ExitBtn2.png");

	Button button0(BackButton,		BackButtonT,	Rectanglef(glm::vec2(0.45f,0.93f),glm::vec2(0.10f,0.06f)), 0.0f);

	buttons.push_back(button0);
}