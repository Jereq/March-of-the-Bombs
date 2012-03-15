#include "MainMeny.h"
#include "Game.h"
#include "LobbyScreen.h"
#include "OptionScreen.h"
#include "CreditsScreen.h"
#include "HtPScreen.h"

MainMeny::MainMeny()
	: game(Game::getInstance())
{
	MainMeny::createBackground();
	MainMeny::createButtons();			//creates the vector holding all the buttons for this screen
}

MainMeny::~MainMeny()
{
}

void MainMeny::atEntry()
{
	game->getSoundManager()->playBackgroundSound("Sounds/Mainbackground.mp3");
}

//Mainmenys updateinfo
void MainMeny::update(float deltaTime)
{
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

				KeyboardEventMethod(keyEvent);
			}
			break;

		case EventType::MouseButton:
			{
				MouseButtonEvent* mbEvent = static_cast<MouseButtonEvent*>(ev.get());				

				MousePressEventMethod(mbEvent);
				
			}
			break;
		case EventType::MouseMove:
			{
				MouseMoveEvent* mmEvent = static_cast<MouseMoveEvent*>(ev.get());

				MouseTouchEventMethod(mmEvent);
			}
			break;
		}
	}
}

//MainMenys drawinfo
void MainMeny::draw(Graphics::ptr graphics)
{
	//starts to render all the buttons
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->render(graphics);
	}
}

Screen::ptr MainMeny::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void MainMeny::createButtons()
{
	TextureSection LobbyButton		(L"images/NewBI/NPBtn1.png");
	TextureSection HtPButton		(L"images/NewBI/HtPBtn1.png");
	TextureSection OptionButton		(L"images/NewBI/OBtn1.png");
	TextureSection CreditsButton	(L"images/NewBI/CBtn1.png");
	TextureSection ExitButton		(L"images/NewBI/ExitBtn1.png");

	TextureSection LobbyButtonT		(L"images/NewBI/NPBtn2.png");
	TextureSection HtPButtonT		(L"images/NewBI/HtPBtn2.png");
	TextureSection OptionButtonT	(L"images/NewBI/OBtn2.png");
	TextureSection CreditsButtonT	(L"images/NewBI/CBtn2.png");
	TextureSection ExitButtonT		(L"images/NewBI/ExitBtn2.png");

	lobbyButton		= Button::ptr(new Button(LobbyButton,	LobbyButtonT,	Rectanglef(glm::vec2(0.30f,0.25f),glm::vec2(0.40f,0.15f)), 0.0f));
	htPButton		= Button::ptr(new Button(HtPButton,		HtPButtonT,		Rectanglef(glm::vec2(0.53f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f));
	optionButton	= Button::ptr(new Button(OptionButton,	OptionButtonT,	Rectanglef(glm::vec2(0.27f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f));
	creditsButton	= Button::ptr(new Button(CreditsButton,	CreditsButtonT,	Rectanglef(glm::vec2(0.80f,0.04f),glm::vec2(0.15f,0.10f)), 0.0f));
	exitButton		= Button::ptr(new Button(ExitButton,		ExitButtonT,	Rectanglef(glm::vec2(0.05f,0.04f),glm::vec2(0.15f,0.10f)), 0.0f));

	buttons.push_back(lobbyButton);
	buttons.push_back(htPButton);
	buttons.push_back(optionButton);
	buttons.push_back(creditsButton);
	buttons.push_back(exitButton);
}

void MainMeny::createBackground()
{
	TextureSection::ptr background(new TextureSection(L"Images/NewBI/Start.png"));
	game->getGraphics()->setBackground(background);
}

void MainMeny::KeyboardEventMethod(KeyboardEvent* keyEvent)
{
	const static char ESC = 0x1B;
	if (keyEvent->key == ESC && keyEvent->keyEventType == KeyboardEventType::Pressed)
	{
		game->close();
	}
}

void MainMeny::MousePressEventMethod(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (lobbyButton->getState() == Hovered)
		{
			nextScreen = Screen::ptr(new LobbyScreen());
			game->getEvents().clear();
		}

		if (htPButton->getState() == Hovered)
		{
			nextScreen = Screen::ptr(new HtPScreen());
			game->getEvents().clear();
		}

		if (optionButton->getState() == Hovered)
		{
			nextScreen = Screen::ptr(new OptionScreen());
			game->getEvents().clear();
		}

		if (creditsButton->getState() == Hovered)
		{
			nextScreen = Screen::ptr(new CreditsScreen());
			game->getEvents().clear();
		}

		if (exitButton->getState() == Hovered)
		{
			game->close();
			game->getEvents().clear();
		}
	}
}

void MainMeny::MouseTouchEventMethod(MouseMoveEvent* mmEvent)
{
	BOOST_FOREACH(Button::ptr& button, buttons)
	{
		if(button->intersects(mmEvent->position))
		{
			button->setState(Hovered);
		}
		else
		{
			button->setState(Unused);
		}
	}
}