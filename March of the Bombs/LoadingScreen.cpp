#include "LoadingScreen.h"

#include "Game.h"
#include "GameScreen.h"

void LoadingScreen::loadGame()
{
	newGame = Screen::ptr(new GameScreen());
}

LoadingScreen::LoadingScreen()
{
	loadThread = boost::thread(boost::bind(&LoadingScreen::loadGame, this));
}

void LoadingScreen::atEntry()
{
	Game::getInstance()->getGraphics()->setBackground(
		TextureSection::ptr(new TextureSection(L"Images/background_loading.png")));
}

void LoadingScreen::update(float deltaTime)
{
	if (!nextScreen)
	{
		if (loadThread.timed_join(boost::posix_time::milliseconds(0)))
		{
			nextScreen = newGame;
		}
	}

	Game::getInstance()->getEvents().clear();
}

void LoadingScreen::draw(Graphics::ptr graphics)
{
}

Screen::ptr LoadingScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}