#include "GameScreen.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "BlockModelData.h"
#include "PlaneModelData.h"
#include "StandardBombModelData.h"

GameScreen::GameScreen()
	: game(Game::getInstance()), cameraPos(new AttachmentPoint(glm::vec3(20, 15, 50), glm::vec3(-30, -45, 0))),
	rotationYSpeed(0), rotationXSpeed(0)
{
	Graphics::ptr graphics = game->getGraphics();

	graphics->getCamera()->setAttachmentPoint(cameraPos);

	light = PointLight::ptr(new PointLight(glm::vec4(35, 150, 35, 1), glm::vec3(1.f)));
	graphics->setLight(light);


	blockMap.loadDefaultMap();

	const static int numBombs = 40;
	for (int i = 0; i < numBombs; i++)
	{
		bombs.push_back(Bomb(0));
		Bomb& addedBomb = bombs.back();

		addedBomb.setPosition(glm::vec3(i * 70.f / numBombs, 0, 0));
		addedBomb.setTarget(blockMap, glm::vec3(20.5f, 0.f, 32.5f));
	}
}

void GameScreen::update(float deltaTime)
{
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

	BOOST_FOREACH(Bomb& bomb, bombs)
	{
		bomb.updatePosition(deltaTime);
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
	BOOST_FOREACH(Bomb& bomb, bombs)
	{
		bomb.draw(graphics);
	}

	blockMap.draw(graphics);
}

Screen::ptr GameScreen::getNextScreen()
{
	return Screen::ptr();
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
			cameraVelocity.x += -10;
			break;

		case 'd':
			cameraVelocity.x += 10;
			break;

		case 'w':
			cameraVelocity.z += -10;
			break;

		case 's':
			cameraVelocity.z += 10;
			break;

		case 'q':
			rotationYSpeed += 30;
			break;

		case 'e':
			rotationYSpeed += -30;
			break;

		case 'r':
			rotationXSpeed += 30;
			break;

		case 'f':
			rotationXSpeed += -30;
			break;

		case 'h':
			BOOST_FOREACH(Bomb& bomb, bombs)
			{
				if (bomb.isSelected())
				{
					bomb.halt();
				}
			}
			break;

		case 'g':
			BOOST_FOREACH(Bomb& bomb, bombs)
			{
				bomb.setSelected(false);
			}
		}
	}

	if (kbEvent->eventType == KeyboardEventType::Released)
	{
		switch (kbEvent->key)
		{
		case 'a':
			cameraVelocity.x -= -10;
			break;

		case 'd':
			cameraVelocity.x -= 10;
			break;

		case 'w':
			cameraVelocity.z -= -10;
			break;

		case 's':
			cameraVelocity.z -= 10;
			break;

		case 'q':
			rotationYSpeed -= 30;
			break;

		case 'e':
			rotationYSpeed -= -30;
			break;

		case 'r':
			rotationXSpeed -= 30;
			break;

		case 'f':
			rotationXSpeed -= -30;
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

		Bomb* hitBomb = NULL;
		BOOST_FOREACH(Bomb& bomb, bombs)
		{
			if (bomb.rayIntersect(origin, direction, distance))
			{
				hitBomb = &bomb;
			}
		}

		if (hitBomb)
		{
			hitBomb->setSelected(!hitBomb->isSelected());
		}
		else
		{
			bool hitGround = blockMap.intersectGround(origin, direction, distance);

			if (hitGround)
			{
				glm::vec3 destination = origin + direction * distance;

				BOOST_FOREACH(Bomb& bomb, bombs)
				{
					if (bomb.isSelected())
					{
						bomb.setTarget(blockMap, destination);
					}
				}
			}
		}
	}
}

void GameScreen::mouseMoveEventHandler(MouseMoveEvent const* mmEvent)
{
}