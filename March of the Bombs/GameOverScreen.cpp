#include "GameOverScreen.h"

#include <boost/lexical_cast.hpp>

#include "Game.h"
#include "MainMeny.h"

void GameOverScreen::goBack()
{
	nextScreen.reset(new MainMeny());
}

GameOverScreen::GameOverScreen(std::string const& myName, std::string const& opponentName,
		unsigned int myScore, unsigned int opponentScore)
	: myName(myName), opponentName(opponentName), myScore(myScore), opponentScore(opponentScore)
{
}

void GameOverScreen::atEntry()
{
	Game::getInstance()->getGraphics()->setBackground(
		TextureSection::ptr(new TextureSection(L"Images/background_game_over.png")));

	mainMenyButton.reset(new Button(
		TextureSection(L"Images/NewBI/BackBtn1.png"),
		TextureSection(L"Images/NewBI/BackBtn2.png"),
		Rectanglef(glm::vec2(0.02f, 0.02f), glm::vec2(0.1f, 0.075f)),
		0));

	glm::vec2 letterRatio(0.5f, 1.f);
	glm::vec3 white(1.f);

	std::string res;
	std::string winningPlayerName;
	std::string losingPlayerName;
	unsigned int winnerScore;
	unsigned int loserScore;

	if (myScore >= opponentScore)
	{
		res = "You Won!";
		winningPlayerName = myName;
		losingPlayerName = opponentName;
		winnerScore = myScore;
		loserScore = opponentScore;
	}
	else
	{
		res = "You Lost!";
		winningPlayerName = opponentName;
		losingPlayerName = myName;
		winnerScore = opponentScore;
		loserScore = myScore;
	}

	result.reset(new Label(
		glm::vec2(0.2f, 0.65f),
		0.15f * letterRatio,
		res,
		0,
		white));

	winningPlayer.reset(new Label(
		glm::vec2(0.15f, 0.4f),
		0.075f * letterRatio,
		winningPlayerName,
		0,
		white));

	losingPlayer.reset(new Label(
		glm::vec2(0.15f, 0.25f),
		0.075f * letterRatio,
		losingPlayerName,
		0,
		white));

	winScore.reset(new Label(
		glm::vec2(0.7f, 0.4f),
		0.075f * letterRatio,
		boost::lexical_cast<std::string>(winnerScore),
		0,
		white));

	loseScore.reset(new Label(
		glm::vec2(0.7f, 0.25f),
		0.075f * letterRatio,
		boost::lexical_cast<std::string>(loserScore),
		0,
		white));
}

void GameOverScreen::update(float deltaTime)
{
	std::deque<Event::ptr>& events = Game::getInstance()->getEvents();
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
}

void GameOverScreen::draw(Graphics::ptr graphics)
{
	mainMenyButton->render(graphics);
	result->draw(graphics);
	winningPlayer->draw(graphics);
	losingPlayer->draw(graphics);
	winScore->draw(graphics);
	loseScore->draw(graphics);
}

Screen::ptr GameOverScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void GameOverScreen::keyboardEventHandler(KeyboardEvent* keyEvent)
{
	const static char ESC = 0x1B;

	if (keyEvent->keyEventType == KeyboardEventType::Pressed)
	{
		switch (keyEvent->key)
		{
		case ESC:
			goBack();
			break;
		}
	}
}

void GameOverScreen::mouseButtonEventHandler(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (mainMenyButton->getState() == Hovered)
		{
			goBack();
		}
	}
}

void GameOverScreen::mouseMoveEventHandler(MouseMoveEvent* mmEvent)
{
	if (mainMenyButton->intersects(mmEvent->position))
	{
		mainMenyButton->setState(Hovered);
	}
	else
	{
		mainMenyButton->setState(Unused);
	}
}