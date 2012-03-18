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

const float GameScreen::TIME_PER_BOMB = 3.f;
const float GameScreen::BASE_POINTS_PER_BOMB = 50.f;
const float GameScreen::FLAG_POINTS_PER_SEC = 5.f;
const float GameScreen::EXPLOSION_RADIUS = 1.5f;
const float GameScreen::FLAG_RADIUS = 5.f;

void GameScreen::spawnBomb(glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity)
{
	if (client)
	{
		Packet::ptr packet = Packet::ptr(new Packet9SpawnBomb(myID, myEntityCount++, position, rotation, velocity));
		client->write(packet);
	}
}

void GameScreen::createExplosion(glm::vec3 const& position, float size, float duration, bool removeBlocks)
{
	// Add new explosion
	explosions.push_back(Explosion(position, glm::vec2(size), duration));
	// Limit explosion sounds to one per frame
	if (explosionsThisFrame < 1)
	{
		game->getSoundManager()->playSound("Sounds/bombexplosion.mp3", position, 30.f);
	}
	explosionsThisFrame++;

	// Find all bombs within the explosion radius
	Bomb::id_set nearbyBombs;
	glm::vec2 groundPos(glm::swizzle<glm::X, glm::Z>(position));
	getNearbyBombs(groundPos, size * 0.5f, nearbyBombs);
	BOOST_FOREACH(Bomb::id const& id, nearbyBombs)
	{
		// Only deal with the players own bombs
		if (id.first == myID)
		{
			Bomb& bomb = myEntities[id.second];
			if (bomb.isAlive())
			{
				Packet::ptr packet(new Packet13RemoveBomb(myID, id.second, true));
				client->write(packet);

				bomb.setIsAlive(false);
			}
		}
	}
	
	// Blow up blocks
	if (removeBlocks)
	{
		float blockExplosionRadius = size * 0.8f;

		glm::vec2 bPos = groundPos - glm::vec2(0.5f);

		glm::ivec2 minPos(glm::floor(bPos - glm::vec2(blockExplosionRadius)));
		glm::ivec2 maxPos(glm::ceil(bPos + glm::vec2(blockExplosionRadius)));

		minPos = glm::max(minPos, 0);
		maxPos = glm::min(maxPos, blockMap.getSize() - glm::ivec2(1));

		std::vector<glm::ivec2> blocks;

		// For every block in a square...
		for (int x = minPos.x; x <= maxPos.x; x++)
		{
			for (int y = minPos.y; y <= maxPos.y; y++)
			{
				glm::ivec2 oPos(x, y);

				Block::ptr block = blockMap.getBlock(oPos);			

				if (block && block->isDestructible() && glm::distance(glm::vec2(oPos), bPos) < blockExplosionRadius)
				{
					blocks.push_back(oPos);
				}
			}
		}

		// If any block to remove was found...
		if (!blocks.empty())
		{
			Packet::ptr packet(new Packet14RemoveBlocks(blocks));
			client->write(packet);
		}
	}

	// Give score for every explosion close to the opponents base
	if (glm::distance(position, opponentBasePos) < size * 0.5f)
	{
		scoreThisFrame += BASE_POINTS_PER_BOMB;
	}
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
		if (id.first == myID)
		{
			playerEntities = &myEntities;
		}
		else if (id.first == opponentID)
		{
			playerEntities = & opponentEntities;
		}
		else
		{
			continue;
		}

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

GameScreen::GameScreen(GameClient::ptr const& client, std::string const& myName, unsigned short myID, glm::vec3 const& myColor,
		std::string const& opponentName, unsigned short opponentID, glm::vec3 const& opponentColor,
		std::string const& mapName, unsigned short myBaseID)
	: game(Game::getInstance()), client(client),
	  rotationYSpeed(0), rotationXSpeed(0), myEntityCount(0), blockMap("Maps/" + mapName + ".txt"),
	  myID(myID), opponentID(opponentID), myColor(myColor), opponentColor(opponentColor),
	  cameraSpeed(20.f), cameraRotationSpeed(45.f), selecting(false),
	  myScore(0), opponentScore(0), myName(myName), opponentName(opponentName), scoreThisFrame(0)
{
	std::vector<glm::ivec2> const& bases = blockMap.getBases();
	assert(bases.size() > myBaseID);
	basePosition = bases[myBaseID];

	std::vector<glm::ivec2> const& flagPos = blockMap.getFlags();
	BOOST_FOREACH(glm::ivec2 const& pos, flagPos)
	{
		flags.push_back(blockMap.getBlock(pos));
	}

	opponentBasePos = dynamic_cast<HQBlock const*>(blockMap.getBlock(bases[1 - myBaseID]).get())->getPosition();

	cameraPos.reset(new AttachmentPoint(glm::vec3(basePosition.x - 20, 30, basePosition.y + 20), glm::vec3(-40, -45, 0)));
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

	timeToNextBomb = TIME_PER_BOMB;
	game->getSoundManager()->playBackgroundSound("Sounds/gamebackground.mp3");
	game->getGraphics()->setBackground(background);
}

void GameScreen::update(float deltaTime)
{
	explosionsThisFrame = 0;

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
		spawnBomb(glm::vec3(basePosition.x + 0.5f, 0, basePosition.y + 0.5f), glm::vec3(0), glm::vec3(0));

		timeToNextBomb += TIME_PER_BOMB;
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
			blockMap.removeBombFromChunk(oldPosition, Bomb::id(myID, entry.first));
			blockMap.addBombToChunk(newPosition, Bomb::id(myID, entry.first));
		}

		if (bomb.hasNewHeading())
		{
			Packet5EntityMove::ptr packet(new Packet5EntityMove(myID, entry.first, bomb.getPosition(), bomb.getRotation(), bomb.getVelocity()));
			client->write(packet);

			bomb.setHasNewHeading(false);
		}

		if (!bomb.hasTarget() && bomb.explodeAtTarget())
		{
			Packet::ptr packet(new Packet13RemoveBomb(myID, entry.first, true));
			client->write(packet);

			bomb.setIsAlive(false);
			continue;
		}

		if (glm::distance(bomb.getPosition(), opponentBasePos) < EXPLOSION_RADIUS * 0.8f)
		{
			Packet::ptr packet(new Packet13RemoveBomb(myID, entry.first, true));
			client->write(packet);

			bomb.setIsAlive(false);
			continue;
		}

		Bomb::id_set nearbyBombs;
		glm::vec2 groundPos(glm::swizzle<glm::X, glm::Z>(bomb.getPosition()));
		getNearbyBombs(groundPos, EXPLOSION_RADIUS * 0.8f, nearbyBombs);
		BOOST_FOREACH(Bomb::id const& id, nearbyBombs)
		{
 			if (id.first == opponentID)
			{
				Packet::ptr packet(new Packet13RemoveBomb(myID, entry.first, true));
				client->write(packet);

				bomb.setIsAlive(false);
				break;
			}
		}
	}

	BOOST_FOREACH(entity_map::value_type& entry, opponentEntities)
	{
		Bomb& bomb = entry.second;

		glm::ivec2 oldPosition(glm::swizzle<glm::X, glm::Z>(bomb.getPosition()));
		bomb.updatePosition(deltaTime);

		glm::ivec2 newPosition(glm::swizzle<glm::X, glm::Z>(bomb.getPosition()));

		if (oldPosition != newPosition)
		{
			blockMap.removeBombFromChunk(oldPosition, Bomb::id(opponentID, entry.first));
			blockMap.addBombToChunk(newPosition, Bomb::id(opponentID, entry.first));
		}
	}

	BOOST_FOREACH(Block::ptr const& bFlag, flags)
	{
		Flag* flag = dynamic_cast<Flag*>(bFlag.get());
		if (flag)
		{
			Bomb::id_set flagBombs;
			glm::vec2 flagGroundPos(glm::swizzle<glm::X, glm::Z>(flag->getPosition()));
			getNearbyBombs(flagGroundPos, FLAG_RADIUS, flagBombs);

			size_t numMyBombs = 0;
			size_t numOpponentBombs = 0;

			BOOST_FOREACH(Bomb::id const& id, flagBombs)
			{
				if (id.first == myID && myEntities[id.second].isAlive())
				{
					numMyBombs++;
				}
				else if(id.first == opponentID && opponentEntities[id.second].isAlive())
				{
					numOpponentBombs++;
				}
			}

			if (numMyBombs > numOpponentBombs)
			{
				flag->setOwner(myID, myColor);
				scoreThisFrame += FLAG_POINTS_PER_SEC * deltaTime;
			}
			else if (numMyBombs < numOpponentBombs)
			{
				flag->setOwner(opponentID, opponentColor);
			}
			else
			{
				flag->setOwner(-1, glm::vec3());
			}
		}
	}

	if (scoreThisFrame != 0)
	{
		myScore += scoreThisFrame;

		Packet::ptr packet(new Packet15UpdatePlayerScore(myID, myScore));
		client->write(packet);

		scoreThisFrame = 0;
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

	if (myScore >= 1000.f)
	{
		nextScreen = Screen::ptr(new MainMeny());
		game->getEvents().clear();
	}
	else if (opponentScore >= 1000.f)
	{
		game->close();
	}
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

					bomb.setIsAlive(false);
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
				selectionPosition = mbEvent->position;
				selecting = true;

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
				Graphics::ptr graphics = game->getGraphics();

				Camera::ptr camera = graphics->getCamera();
				glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();

				glm::mat4 pickingMatrix = glm::inverse(viewProjectionMatrix);

				glm::vec4 nearPosition = pickingMatrix * glm::vec4(mbEvent->position.x * 2 - 1, mbEvent->position.y * 2 - 1, 0, 1);
				nearPosition /= nearPosition.w;

				glm::vec3 origin = cameraPos->getPosition();
				glm::vec3 direction = glm::normalize(glm::vec3(nearPosition) - origin);

				float distance = std::numeric_limits<float>::infinity();

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
	else
	{
		if (mbEvent->button == MouseButton::Left)
		{
			if (selecting)
			{
				float selectionSize = glm::distance(selectionPosition, mbEvent->position);
				if (selectionSize > 0.01f)
				{
					selectBombsBox(selectionPosition, mbEvent->position);
				}
				else
				{
					selectBombRay(mbEvent->position);
				}

				selecting = false;
			}
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

	TextureSection guiBackground(L"Images/NewBI/Start.png");
	SimpleImage Background1(guiBackground, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(0.20f,0.20f)), 0.10f);

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

			glm::ivec2 oldPosition(glm::swizzle<glm::X, glm::Z>(entity.getPosition()));

			entity.setPosition(packet5->getPosition());
			entity.setRotation(packet5->getRotation());
			entity.setVelocity(packet5->getVelocity());

			glm::ivec2 newPosition(glm::swizzle<glm::X, glm::Z>(entity.getPosition()));

			if (oldPosition != newPosition)
			{
				blockMap.removeBombFromChunk(oldPosition, Bomb::id(playerID, entityID));
				blockMap.addBombToChunk(newPosition, Bomb::id(playerID, entityID));
			}
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

		Bomb newBomb(myID, myColor);
		newBomb.setPosition(packet9->getPosition());
		newBomb.setRotation(packet9->getRotation());
		newBomb.setVelocity(packet9->getVelocity());

		glm::vec2 offset = glm::circularRand(1.5f) + glm::diskRand(0.6f);
		newBomb.setTarget(blockMap, newBomb.getPosition() + glm::vec3(offset.x, 0, offset.y));

		myEntities[entityID] = newBomb;
		
		glm::ivec2 block(glm::swizzle<glm::X, glm::Z>(newBomb.getPosition()));
		blockMap.addBombToChunk(block, Bomb::id(playerID, entityID));
	}
	else if (playerID == opponentID)
	{
		unsigned short entityID = packet9->getEntityID();

		Bomb newBomb(opponentID, opponentColor);
		newBomb.setPosition(packet9->getPosition());
		newBomb.setRotation(packet9->getRotation());
		newBomb.setVelocity(packet9->getVelocity());

		opponentEntities[entityID] = newBomb;
		
		glm::ivec2 block(glm::swizzle<glm::X, glm::Z>(newBomb.getPosition()));
		blockMap.addBombToChunk(block, Bomb::id(playerID, entityID));
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
	const static float EXPLOSION_DURATION(0.3f);

	if (playerID == myID)
	{
		if (myEntities.count(entityID) == 1)
		{
			Bomb const& bomb = myEntities[entityID];

			glm::ivec2 block(glm::swizzle<glm::X, glm::Z>(bomb.getPosition()));
			blockMap.removeBombFromChunk(block, Bomb::id(playerID, entityID));

			if (packet13->getExplode())
			{
				createExplosion(bomb.getPosition() + EXPLOSION_OFFSET, EXPLOSION_RADIUS * 2, EXPLOSION_DURATION, true);
			}

			myEntities.erase(entityID);
		}
	}
	else if (playerID == opponentID)
	{
		if (opponentEntities.count(entityID) == 1)
		{
			Bomb const& bomb = opponentEntities[entityID];

			glm::ivec2 block(glm::swizzle<glm::X, glm::Z>(bomb.getPosition()));
			blockMap.removeBombFromChunk(block, Bomb::id(playerID, entityID));

			if (packet13->getExplode())
			{
				createExplosion(bomb.getPosition() + EXPLOSION_OFFSET, EXPLOSION_RADIUS * 2, EXPLOSION_DURATION, false);
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

void GameScreen::handlePacket15UpdatePlayerScore(Packet15UpdatePlayerScore::const_ptr const& packet)
{
	Packet15UpdatePlayerScore const* packet15 = static_cast<Packet15UpdatePlayerScore const*>(packet.get());

	if (packet15->getPlayerID() == opponentID)
	{
		opponentScore = packet15->getNewScore();
	}
}