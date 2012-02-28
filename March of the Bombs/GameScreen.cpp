#include "GameScreen.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "BlockModel.h"
#include "PlaneModel.h"
#include "StandardBombModelData.h"

GameScreen::GameScreen()
	: game(Game::getInstance()), cameraPos(new AttachmentPoint(glm::vec3(-20, 15, 20), glm::vec3(-30, -45, 0))),
	rotationYSpeed(0), rotationXSpeed(0)
{
	game->getGraphics()->getCamera()->setAttachmentPoint(cameraPos);
	
	for (int i = 0; i < 100; i++)
	{
		test[i] = Model::ptr(new Model(StandardBombModelData::getInstance()));
		test[i]->setPosition(glm::vec3((i % 10 - 4.5f) * 2.f, 0, (-(i / 10) + 4.5f) * 2.f));
		test[i]->setScale(glm::vec3(0.5f));
		test[i]->setRotation(glm::vec3(0, 90, 0));
	}

	for (int i = 0; i < 25; i++)
	{
		testBlocks[i] = Model::ptr(new BlockModel());
		testBlocks[i]->setPosition(glm::vec3(i - 13.f, 0, -13.f));
	}

	for (int i = 25; i < 50; i++)
	{
		testBlocks[i] = Model::ptr(new BlockModel());
		testBlocks[i]->setPosition(glm::vec3(12.f, 0, i - 25 - 13.f));
	}

	for (int i = 50; i < 75; i++)
	{
		testBlocks[i] = Model::ptr(new BlockModel());
		testBlocks[i]->setPosition(glm::vec3(-(i - 50) + 12.f, 0, 12.f));
	}

	for (int i = 75; i < 100; i++)
	{
		testBlocks[i] = Model::ptr(new BlockModel());
		testBlocks[i]->setPosition(glm::vec3(-13.f, 0, -(i - 75) + 12.f));
	}

	test[0] = Model::ptr(new PlaneModel());
	test[0]->setPosition(glm::vec3(-100, 0, -100));
	test[0]->setScale(glm::vec3(200));

	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(0, 20, -5, 1), glm::vec3(0.2f))));
	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(0, 20, 0, 1), glm::vec3(0.2f))));
	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(-5, 60, -5, 1), glm::vec3(0.7f))));
	game->getGraphics()->setPrimaryLights(primaryLights);
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


	//for (int i = 1; i < 100; i++)
	//{
	//	glm::vec3 rot = test[i]->getRotation() + glm::vec3(17, 67, 0) * deltaTime;
	//	test[i]->setRotation(rot);
	//}

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
	for (int i = 0; i < 100; i++)
	{
		graphics->drawModel(test[i]);
	}

	for (int i = 0; i < 100; i++)
	{
		graphics->drawModel(testBlocks[i]);
	}
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