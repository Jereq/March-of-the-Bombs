#include "GameScreen.h"

#include "Game.h"

GameScreen::GameScreen()
{
	for (int i = 0; i < 50; i++)
	{
		test[i] = Model::ptr(new Model("Models/StandardBomber1.3ds"));
		test[i]->setPosition(glm::vec3(i % 10 - 4.5f, (i / 10) * 0.1f - 0.45f, -(i / 10)) * 3.f);
	}

	for (int i = 50; i < 100; i++)
	{
		test[i] = Model::ptr(new Model("Models/StandardBomber1.3ds"));
		test[i]->setPosition(glm::vec3(i % 10 - 4.5f, (i / 10) * 0.1f + 1.45f, -(i / 10) + 5) * 3.f);
	}

	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(0, 20, -5, 1), glm::vec3(0.4f))));
	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(0, 20, 0, 1), glm::vec3(0.4f))));
	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(-5, 20, 0, 1), glm::vec3(0.4f))));
	Game::getInstance()->getGraphics()->setPrimaryLights(primaryLights);
}

void GameScreen::update(float deltaTime)
{
	for (int i = 0; i < 100; i++)
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
}

Screen::ptr GameScreen::getNextScreen()
{
	return Screen::ptr();
}