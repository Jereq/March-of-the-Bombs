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

void GameScreen::spawnBomb(glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity)
{
}

void GameScreen::createExplosion(glm::vec3 const& position, float size, float duration, bool removeBlocks)
{
	MChannel* sound = nullptr;

	if (explosionsThisFrame < 1)
	{
		sound = game->getSoundManager()->playSound("Sounds/bombexplosion.mp3", position, 30.f);
	}

	// Add new explosion
	explosions.push_back(Explosion(position, glm::vec2(size), duration, sound));
	// Limit explosion sounds to one per frame
	explosionsThisFrame++;
}

void GameScreen::preloadTextures()
{
	preloadedTextures.insert(TextureSection::ptr(new TextureSection(L"Images/explosion.png")));
}

void GameScreen::selectBombsBox(glm::vec2 const& pos1, glm::vec2 const& pos2)
{
	Graphics::ptr graphics = game->getGraphics();

	Camera::ptr camera = graphics->getCamera();
	glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();

	glm::mat4 pickingMatrix = glm::inverse(viewProjectionMatrix);

	glm::vec2 lowerLeft = glm::min(pos1, pos2);
	glm::vec2 upperRight = glm::max(pos1, pos2);

	glm::vec4 nearPositionLL = pickingMatrix * glm::vec4(lowerLeft.x  * 2 - 1, lowerLeft.y  * 2 - 1, 0, 1);
	glm::vec4 nearPositionLR = pickingMatrix * glm::vec4(upperRight.x * 2 - 1, lowerLeft.y  * 2 - 1, 0, 1);
	glm::vec4 nearPositionUL = pickingMatrix * glm::vec4(lowerLeft.x  * 2 - 1, upperRight.y * 2 - 1, 0, 1);
	glm::vec4 nearPositionUR = pickingMatrix * glm::vec4(upperRight.x * 2 - 1, upperRight.y * 2 - 1, 0, 1);
	glm::vec4 farPositionLL  = pickingMatrix * glm::vec4(lowerLeft.x  * 2 - 1, lowerLeft.y  * 2 - 1, 1, 1);
	glm::vec4 farPositionLR  = pickingMatrix * glm::vec4(upperRight.x * 2 - 1, lowerLeft.y  * 2 - 1, 1, 1);
	glm::vec4 farPositionUL  = pickingMatrix * glm::vec4(lowerLeft.x  * 2 - 1, upperRight.y * 2 - 1, 1, 1);
	glm::vec4 farPositionUR  = pickingMatrix * glm::vec4(upperRight.x * 2 - 1, upperRight.y * 2 - 1, 1, 1);

	nearPositionLL /= nearPositionLL.w;
	nearPositionLR /= nearPositionLR.w;
	nearPositionUL /= nearPositionUL.w;
	nearPositionUR /= nearPositionUR.w;
	farPositionLL  /= farPositionLL.w;
	farPositionLR  /= farPositionLR.w;
	farPositionUL  /= farPositionUL.w;
	farPositionUR  /= farPositionUR.w;

	Frustum selectionFrustum = Frustum(
		glm::vec3(nearPositionLL),
		glm::vec3(nearPositionLR),
		glm::vec3(nearPositionUL),
		glm::vec3(nearPositionUR),
		glm::vec3(farPositionLL),
		glm::vec3(farPositionLR),
		glm::vec3(farPositionUL),
		glm::vec3(farPositionUR));

	BOOST_FOREACH(entity_map::value_type& entry, myEntities)
	{
		Bomb& bomb = entry.second;
		bomb.setSelected(bomb.frustumIntersect(selectionFrustum));
	}
}

void GameScreen::selectBombRay(glm::vec2 const& pos)
{
	Graphics::ptr graphics = game->getGraphics();

	Camera::ptr camera = graphics->getCamera();
	glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();

	glm::mat4 pickingMatrix = glm::inverse(viewProjectionMatrix);

	glm::vec4 nearPosition = pickingMatrix * glm::vec4(pos.x * 2 - 1, pos.y * 2 - 1, 0, 1);
	nearPosition /= nearPosition.w;

	glm::vec3 origin = cameraPos->getPosition();
	glm::vec3 direction = glm::normalize(glm::vec3(nearPosition) - origin);

	float distance = std::numeric_limits<float>::infinity();

	Bomb* hitBomb = NULL;
	BOOST_FOREACH(entity_map::value_type& bomb, myEntities)
	{
		bomb.second.setSelected(false);

		if (bomb.second.rayIntersect(origin, direction, distance))
		{
			hitBomb = &bomb.second;
		}
	}

	if (hitBomb)
	{
		hitBomb->setSelected(true);
	}
}

void GameScreen::getNearbyBombs(glm::vec2 const& center, float distance, Bomb::id_set& res) const
{
	Bomb::id_set tRes;
	blockMap.getNearbyBombs(center, glm::vec2(distance), tRes);

	BOOST_FOREACH(Bomb::id const& id, tRes)
	{
		entity_map const* playerEntities;
		playerEntities = &myEntities;

		if (playerEntities->count(id.second) != 1)
		{
			return;
		}

		Bomb const& bomb = playerEntities->at(id.second);
		if (glm::distance(center, bomb.getPosition().swizzle(glm::X, glm::Z)) <= distance)
		{
			res.insert(id);
		}
	}
}

GameScreen::GameScreen()
	: game(Game::getInstance()),
	  rotationYSpeed(0), rotationXSpeed(0), myEntityCount(0), blockMap("Maps/emptyplane 75x75.txt"),
	  cameraSpeed(5.67f), cameraRotationSpeed(180.f)
{
	std::vector<glm::ivec2> const& bases = blockMap.getBases();
	assert(bases.size() > 0);
	glm::ivec2 basePosition = bases[0];

	std::vector<glm::ivec2> const& flagPos = blockMap.getFlags();
	BOOST_FOREACH(glm::ivec2 const& pos, flagPos)
	{
		flags.push_back(blockMap.getBlock(pos));
	}

	cameraPos.reset(new AttachmentPoint(glm::vec3(basePosition.x, 1.8f, basePosition.y + 2.f), glm::vec3(-10.f, 180.f, 0.f)));
}

void GameScreen::atEntry()
{
	preloadTextures();

	createBackground();
	createButtons();

	Graphics::ptr graphics = game->getGraphics();
	graphics->getCamera()->setAttachmentPoint(cameraPos);

	glm::ivec2 mapSize = blockMap.getSize();

	light = PointLight::ptr(
		new PointLight(
			glm::vec4(mapSize.x / 2.f, 70, mapSize.y / 2.f, 1),
			glm::vec4(0, -1, 0, 0),
			glm::vec3(1.f)));

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

	graphics->setLight(light);

	game->getSoundManager()->playBackgroundSound("Sounds/gamebackground.mp3", 0.5f);
	game->getGraphics()->setBackground(background);

	//createExplosion(glm::vec3(0,0,0),1.f,5.f,true);

	boost::shared_ptr<LoudEntity> entity;

	entity.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/fire-1.mp3"));
	entity->setPosition(glm::vec3(38.f, 0.f, 33.f));

	loudEntities.push_back(boost::shared_ptr<LoudEntity>(entity));

	entity.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/Chipmunks_sound-Uday-1052330468.mp3"));
	entity->setPosition(glm::vec3(60.f, 0.f, 22.f));

	loudEntities.push_back(boost::shared_ptr<LoudEntity>(entity));

	entity.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/Cartoon Voice Baritone-SoundBible.com-2068310080.mp3"));
	entity->setPosition(glm::vec3(58.f, 0.f, 54.f));

	loudEntities.push_back(boost::shared_ptr<LoudEntity>(entity));

	zombie.reset(new LoudEntity(game->getSoundManager().get(), "Sounds/Zombie Gibberish-SoundBible.com-589887278.mp3"));

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

void GameScreen::update(float deltaTime)
{
	explosionsThisFrame = 0;

	if (nextScreen)
	{
		return;
	}

	currentDeltaTime = deltaTime;

	std::deque<Event::ptr>& events = game->getEvents();
	std::deque<Event::ptr>::size_type numEvents = events.size();
	std::deque<Event::ptr>::size_type eventsHandled = 0;

	while (!events.empty() && eventsHandled++ < numEvents)
	{
		Event::ptr ev = events.front();
		events.pop_front();

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

	BOOST_FOREACH(entity_map::value_type& entry, myEntities)
	{
		Bomb& bomb = entry.second;

		if (!bomb.isAlive())
		{
			continue;
		}

		glm::ivec2 oldPosition(glm::swizzle<glm::X, glm::Z>(bomb.getPosition()));
		bomb.updatePosition(deltaTime);

		glm::ivec2 newPosition(glm::swizzle<glm::X, glm::Z>(bomb.getPosition()));

		if (oldPosition != newPosition)
		{
			blockMap.removeBombFromChunk(oldPosition, Bomb::id(0, entry.first));
			blockMap.addBombToChunk(newPosition, Bomb::id(0, entry.first));
		}
	}

	glm::vec3 playerPos(cameraPos->getPosition());
	playerPos.y = 0.f;
	glm::vec3 zombieOffset(playerPos - zombie->getPosition());

	const static float ZOMBIE_SPEED = 1.3f;

	if (glm::length(zombieOffset) > ZOMBIE_SPEED * deltaTime)
	{
		zombieOffset = glm::normalize(zombieOffset);
		zombie->setPosition(zombie->getPosition() + zombieOffset * ZOMBIE_SPEED * deltaTime);
	}
	else
	{
		zombie->setPosition(playerPos);
	}

	if (zombieOffset != glm::vec3())
	{
		zombie->setDirection(zombieOffset);
	}

	std::list<Explosion>::iterator it = explosions.begin();
	while (it != explosions.end())
	{
		it->update(deltaTime);

		if (it->isDone())
		{
			it = explosions.erase(it);
		}
		else
		{
			++it;
		}
	}

	glm::vec3 rotation = cameraPos->getRotation();
	rotation.y += rotationYSpeed * deltaTime;
	rotation.x += rotationXSpeed * deltaTime;
	if (rotation.x > 35.f)
	{
		rotation.x = 35.f;
	}
	else if (rotation.x < -90)
	{
		rotation.x = -90;
	}

	cameraPos->setRotation(rotation);

	glm::mat3 rotationMatrix = glm::mat3(glm::rotate(glm::mat4(), rotation.y, glm::vec3(0, 1, 0)));
	cameraPos->setPosition(cameraPos->getPosition() + rotationMatrix * cameraVelocity * deltaTime);
}

void GameScreen::draw(Graphics::ptr graphics)
{
	BOOST_FOREACH(entity_map::value_type& entry, myEntities)
	{
		entry.second.draw(graphics);
	}

	BOOST_FOREACH(Explosion& exp, explosions)
	{
		exp.draw(graphics);
	}

	for (auto& entity : loudEntities)
	{
		entity->draw(graphics);
	}

	blockMap.draw(graphics);

	//starts to render all the backgrounds
	GuiBackground->render(graphics);

	//starts to render all the buttons
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i].render(graphics);
	}
}

Screen::ptr GameScreen::getNextScreen()
{
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
			{
				//testbutton
				if (buttons[0].getState() == Hovered)
				{
					nextScreen = Screen::ptr(new MainMeny());
					game->getEvents().clear();
				}
			}
			break;
		}
	}
}

void GameScreen::mouseMoveEventHandler(MouseMoveEvent const* mmEvent)
{
	BOOST_FOREACH(Button& button, buttons)
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