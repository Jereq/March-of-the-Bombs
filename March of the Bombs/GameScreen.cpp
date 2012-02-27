#include "GameScreen.h"

#include "Game.h"
#include "BlockModel.h"
#include "PlaneModel.h"

GameScreen::GameScreen()
{
	for (int i = 0; i < 100; i++)
	{
		test[i] = Model::ptr(new Model("Models/StandardBomber1.3ds"));
		test[i]->setPosition(glm::vec3((i % 10 - 4.5f) * 2.f, 0.4f, (-(i / 10) + 4.5f) * 2.f));
		test[i]->setScale(glm::vec3(0.3f));
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
	Game::getInstance()->getGraphics()->setPrimaryLights(primaryLights);
}

void GameScreen::update(float deltaTime)
{
	for (int i = 1; i < 100; i++)
	{
		glm::vec3 rot = test[i]->getRotation() + glm::vec3(17, 67, 0) * deltaTime;
		test[i]->setRotation(rot);
	}

	glm::vec4 pos = primaryLights[0]->getPosition();
	pos += glm::vec4(glm::vec3(0.5f, 0, 0) * deltaTime, 0);
	primaryLights[0]->setPosition(pos);
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