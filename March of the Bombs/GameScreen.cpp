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
	game->getGraphics()->getCamera()->setAttachmentPoint(cameraPos);

	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(35, 150, 35, 1), glm::vec3(1.f))));
	game->getGraphics()->setPrimaryLights(primaryLights);


	blockMap.loadDefaultMap();

	for (int i = 0; i < testCount; i++)
	{
		test[i] = Model::ptr(new Model(StandardBombModelData::getInstance()));
		test[i]->setPosition(glm::vec3(i * 70.f / testCount, 0, 0));
		test[i]->setScale(glm::vec3(0.3f));

		blockMap.findPath(test[i]->getPosition().swizzle(glm::X, glm::Z), glm::vec2(20.5f, 32.5f), testPath[i]);
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

	for (int i = 0; i < testCount; i++)
	{
		if (!testPath[i].empty())
		{
			const static float speed = 3.f;
			glm::vec3 pos = test[i]->getPosition();
			float distanceToGo = speed * deltaTime;

			while (distanceToGo > 0 && !testPath[i].empty())
			{
				float distance = glm::distance(pos, testPath[i].front());

				if (distanceToGo >= distance)
				{
					distanceToGo -= distance;
					pos = testPath[i].front();
					testPath[i].pop_front();
				}
				else
				{
					glm::vec3 direction = glm::normalize(testPath[i].front() - pos);
					pos += direction * distanceToGo;
					test[i]->setRotation(glm::vec3(0, glm::degrees(glm::atan(-direction.z, direction.x)), 0));
					distanceToGo = 0;
				}
			}

			test[i]->setPosition(pos);
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
	for (int i = 0; i < testCount; i++)
	{
		graphics->drawModel(test[i]);
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
}

void GameScreen::mouseMoveEventHandler(MouseMoveEvent const* mmEvent)
{
}