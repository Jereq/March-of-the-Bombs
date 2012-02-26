#include "GameScreen.h"

#include "Game.h"

GameScreen::GameScreen()
{
	test = Model::ptr(new Model(Model3DS::ptr(new Model3DS("Models/StandardBomber1.3ds"))));
	test->setScale(glm::vec3(0.5f));

	primaryLights.push_back(PointLight::ptr(new PointLight(glm::vec4(0, 20, 0, 1), glm::vec3(1))));
	Game::getInstance()->getGraphics()->setPrimaryLights(primaryLights);
}

void GameScreen::update(float deltaTime)
{
	glm::vec3 rot = test->getRotation() + glm::vec3(0.0001f, 0.00003, 0);
	test->setRotation(rot);
}

void GameScreen::draw(Graphics::ptr graphics)
{
	graphics->drawModel(test);
}

Screen::ptr GameScreen::getNextScreen()
{
	return Screen::ptr();
}