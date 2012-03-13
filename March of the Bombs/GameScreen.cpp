#include "GameScreen.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include "Game.h"
#include "MainMeny.h"
#include "BlockModelData.h"
#include "PlaneModelData.h"
#include "StandardBombModelData.h"

const float GameScreen::TIME_PER_BOMB = 3.f;

void GameScreen::spawnBomb(glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity)
{
	if (client)
	{
		Packet::ptr packet = Packet::ptr(new Packet9SpawnBomb(myID, myEntityCount++, position, rotation, velocity));
		client->write(packet);
	}
}

void GameScreen::preloadTextures()
{
	preloadedTextures.insert(TextureSection::ptr(new TextureSection(L"Images/explosion.png")));
}

GameScreen::GameScreen(GameClient::ptr const& client, std::string const& mapName, unsigned short myID,
		unsigned short opponentID, unsigned short myBaseID, glm::vec3 const& opponentColor)
	: game(Game::getInstance()), client(client),
	  rotationYSpeed(0), rotationXSpeed(0), myEntityCount(0), blockMap(mapName + ".txt"),
	  myID(myID), opponentID(opponentID), opponentColor(opponentColor), cameraSpeed(20.f), cameraRotationSpeed(45.f)
{
	std::vector<glm::ivec2> const& bases = blockMap.getBases();
	assert(bases.size() > myBaseID);
	basePosition = bases[myBaseID];

	cameraPos.reset(new AttachmentPoint(glm::vec3(basePosition.x - 20, 30, basePosition.y + 20), glm::vec3(-40, -45, 0)));
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
	mapPoints.push_back(glm::vec3(      0.f, 1.f,       0.f));
	mapPoints.push_back(glm::vec3(      0.f, 0.f, mapSize.y));
	mapPoints.push_back(glm::vec3(      0.f, 1.f, mapSize.y));
	mapPoints.push_back(glm::vec3(mapSize.x, 0.f,       0.f));
	mapPoints.push_back(glm::vec3(mapSize.x, 1.f,       0.f));
	mapPoints.push_back(glm::vec3(mapSize.x, 0.f, mapSize.y));
	mapPoints.push_back(glm::vec3(mapSize.x, 1.f, mapSize.y));
	light->setPoints(mapPoints);

	graphics->setLight(light);

	preloadTextures();

	GameScreen::createBackground();
	GameScreen::createButtons();
}

void GameScreen::atEntry()
{
	timeToNextBomb = TIME_PER_BOMB;
}

void GameScreen::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets())
		{
			Packet::ptr packet = client->popReceivedPacket();
			packet->dispatch(&shared_from_this());
		}
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

	timeToNextBomb -= deltaTime;
	while (timeToNextBomb <= 0)
	{
		spawnBomb(glm::vec3(basePosition.x, 0, basePosition.y), glm::vec3(0), glm::vec3(0));

		timeToNextBomb += TIME_PER_BOMB;
	}

	BOOST_FOREACH(entity_map::value_type& entry, myEntities)
	{
		Bomb& bomb = entry.second;
		bomb.updatePosition(deltaTime);

		if (bomb.hasNewHeading())
		{
			Packet5EntityMove::ptr packet(new Packet5EntityMove(myID, entry.first, bomb.getPosition(), bomb.getRotation(), bomb.getVelocity()));
			client->write(packet);

			bomb.setHasNewHeading(false);
		}
	}

	BOOST_FOREACH(entity_map::value_type& entry, opponentEntities)
	{
		entry.second.updatePosition(deltaTime);
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
	if (rotation.x > 0)
	{
		rotation.x = 0;
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

	BOOST_FOREACH(entity_map::value_type& entry, opponentEntities)
	{
		entry.second.draw(graphics);
	}

	BOOST_FOREACH(Explosion& exp, explosions)
	{
		exp.draw(graphics);
	}

	blockMap.draw(graphics);

	//starts to render all the backgrounds
	for(unsigned int i = 0; i < Backgrounds.size(); i++)
	{
		Backgrounds[i].render(graphics);
	}

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
	if (kbEvent->eventType == KeyboardEventType::Pressed)
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

		case 'h':
			BOOST_FOREACH(entity_map::value_type& entry, myEntities)
			{
				Bomb& bomb = entry.second;
				if (bomb.isSelected())
				{
					bomb.halt();
					
					Packet::ptr packet(new Packet5EntityMove(myID, entry.first, bomb.getPosition(), bomb.getRotation(), bomb.getVelocity()));
					client->write(packet);
				}
			}
			break;

		case 'g':
			BOOST_FOREACH(entity_map::value_type& bomb, myEntities)
			{
				bomb.second.setSelected(false);
			}
			break;

		case ' ':
			BOOST_FOREACH(entity_map::value_type& entry, myEntities)
			{
				Bomb& bomb = entry.second;
				if (bomb.isSelected())
				{
					Packet::ptr packet(new Packet13RemoveBomb(myID, entry.first, true));
					client->write(packet);

					glm::ivec2 pos(glm::round(bomb.getPosition().swizzle(glm::X, glm::Z)));

					const static glm::ivec2 offsets[4] =
					{
						glm::ivec2(-1, -1),
						glm::ivec2(-1,  0),
						glm::ivec2( 0, -1),
						glm::ivec2( 0,  0)
					};

					std::vector<glm::ivec2> blocks;

					BOOST_FOREACH(glm::ivec2 const& offset, offsets)
					{
						glm::ivec2 oPos(pos + offset);
						Block::ptr block = blockMap.getBlock(oPos);

						if (block && block->isDestructible())
						{
							blocks.push_back(oPos);
						}
					}

					if (!blocks.empty())
					{
						packet.reset(new Packet14RemoveBlocks(blocks));
						client->write(packet);
					}
				}
			}
			break;

		case '+':
			cameraRotationSpeed += 5;
			break;

		case '-':
			cameraRotationSpeed -= 5;
			break;
		}
	}

	if (kbEvent->eventType == KeyboardEventType::Released)
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
		Graphics::ptr graphics = game->getGraphics();

		Camera::ptr camera = graphics->getCamera();
		glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();

		glm::mat4 pickingMatrix = glm::inverse(viewProjectionMatrix);

		glm::vec4 nearPosition = pickingMatrix * glm::vec4(mbEvent->position.x * 2 - 1, mbEvent->position.y * 2 - 1, 0, 1);
		nearPosition /= nearPosition.w;

		glm::vec3 origin = cameraPos->getPosition();
		glm::vec3 direction = glm::normalize(glm::vec3(nearPosition) - origin);

		float distance = std::numeric_limits<float>::infinity();

		switch (mbEvent->button)
		{
		case MouseButton::Left:
			{
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

				//testbutton
				if (buttons[0].getState() == Hovered)
				{
					nextScreen = Screen::ptr(new MainMeny());
					game->getEvents().clear();
				}
			}
			break;

		case  MouseButton::Right:
			{
				bool hitGround = blockMap.intersectGround(origin, direction, distance);

				if (hitGround)
				{
					glm::vec3 destination = origin + direction * distance;

					BOOST_FOREACH(entity_map::value_type& entry, myEntities)
					{
						Bomb& bomb = entry.second;

						if (bomb.isSelected())
						{
							bomb.setTarget(blockMap, destination);
					
							Packet::ptr packet(new Packet5EntityMove(myID, entry.first, bomb.getPosition(), bomb.getRotation(), bomb.getVelocity()));
							client->write(packet);
						}
					}
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
	TextureSection Background(L"images/NewBI/Start.png");
	game->getGraphics()->setBackground(TextureSection::ptr(new TextureSection(Background)));

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(0.20f,0.20f)), 0.10f);

	Backgrounds.push_back(Background1);
}

void GameScreen::createButtons()
{
	TextureSection BackButton(L"images/NewBI/BackBtn1.png");
	TextureSection BackButtonT(L"images/NewBI/BackBtn2.png");

	Button button0(BackButton,		BackButtonT,	Rectanglef(glm::vec2(0.05f,0.05f),glm::vec2(0.10f,0.05f)), 0.0f);

	buttons.push_back(button0);
}

void GameScreen::handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet)
{
	Packet5EntityMove const* packet5 = static_cast<Packet5EntityMove const*>(packet.get());

	unsigned short playerID = packet5->getPlayerID();

	if (playerID == myID)
	{
		// No need to move my bombs... :)
	}
	else if (playerID == opponentID)
	{
		unsigned short entityID = packet5->getEntityID();

		if (opponentEntities.count(entityID) == 1)
		{
			Bomb& entity = opponentEntities[entityID];

			entity.setPosition(packet5->getPosition());
			entity.setRotation(packet5->getRotation());
			entity.setVelocity(packet5->getVelocity());
		}
	}
	else
	{
		std::cerr << "Error: Received packet with unknown player ID" << std::endl;
	}
}

void GameScreen::handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet)
{
	Packet9SpawnBomb const* packet9 = static_cast<Packet9SpawnBomb const*>(packet.get());

	unsigned short playerID = packet9->getPlayerID();

	if (playerID == myID)
	{
		unsigned short entityID = packet9->getEntityID();

		Bomb newBomb(myID);
		newBomb.setPosition(packet9->getPosition());
		newBomb.setRotation(packet9->getRotation());
		newBomb.setVelocity(packet9->getVelocity());

		glm::vec2 offset = glm::circularRand(1.5f) + glm::diskRand(0.6f);
		newBomb.setTarget(blockMap, newBomb.getPosition() + glm::vec3(offset.x, 0, offset.y));

		myEntities[entityID] = newBomb;
	}
	else if (playerID == opponentID)
	{
		unsigned short entityID = packet9->getEntityID();

		Bomb newBomb(myID);
		newBomb.setPosition(packet9->getPosition());
		newBomb.setRotation(packet9->getRotation());
		newBomb.setVelocity(packet9->getVelocity());

		opponentEntities[entityID] = newBomb;
	}
	else
	{
		std::cerr << "Error: Received packet with unknown player ID" << std::endl;
	}
}

void GameScreen::handlePacket13RemoveBomb(Packet13RemoveBomb::const_ptr const& packet)
{
	Packet13RemoveBomb const* packet13 = static_cast<Packet13RemoveBomb const*>(packet.get());

	unsigned short playerID = packet13->getPlayerID();
	unsigned short entityID = packet13->getEntityID();

	const static glm::vec3 EXPLOSION_OFFSET(0, 0.3f, 0);
	const static glm::vec2 EXPLOSION_SIZE(3.f);
	const static float EXPLOSION_DURATION = 0.3f;

	if (playerID == myID)
	{
		if (myEntities.count(entityID) == 1)
		{
			if (packet13->getExplode())
			{
				Bomb const& bomb = myEntities[entityID];
				explosions.push_back(Explosion(bomb.getPosition() + EXPLOSION_OFFSET, EXPLOSION_SIZE, EXPLOSION_DURATION));
			}

			myEntities.erase(entityID);
		}
	}
	else if (playerID == opponentID)
	{
		if (opponentEntities.count(entityID) == 1)
		{
			if (packet13->getExplode())
			{
				Bomb const& bomb = opponentEntities[entityID];
				explosions.push_back(Explosion(bomb.getPosition() + EXPLOSION_OFFSET, EXPLOSION_SIZE, EXPLOSION_DURATION));
			}

			opponentEntities.erase(entityID);
		}
	}
	else
	{
		std::cerr << "Error: Received packet with unknown player ID" << std::endl;
	}
}

void GameScreen::handlePacket14RemoveBlocks(Packet14RemoveBlocks::const_ptr const& packet)
{
	Packet14RemoveBlocks const* packet14 = static_cast<Packet14RemoveBlocks const*>(packet.get());

	std::vector<glm::ivec2> const& blocks = packet14->getBlocks();

	BOOST_FOREACH(glm::ivec2 const& block, blocks)
	{
		blockMap.removeBlock(block);
	}
}