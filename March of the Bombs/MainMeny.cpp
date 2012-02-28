#include "MainMeny.h"
#include "Game.h"
#include <boost/foreach.hpp>
//#include "KeyboardEvent.h"
//#include "MouseButtonEvent.h"
#include "GameScreen.h"

MainMeny::MainMeny()
	: game(Game::getInstance())
{
	MainMeny::createBackground();				//creates the vector holding all the backgrounds for this screen
	MainMeny::createButtons();					//creates the vector holding all the buttons for this screen
}

MainMeny::~MainMeny()
{
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

Screen::ptr MainMeny::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void MainMeny::createButtons()
{
	//GLTexture::ptr TestButton1	=	GLTexture::loadTexture(L"images/skull.tga");
	GLTexture::ptr LobbyButton		=	GLTexture::getTexture(L"images/ButtonLobby.png");
	GLTexture::ptr HtPButton		=	GLTexture::getTexture(L"images/ButtonHtP.png");
	GLTexture::ptr OptionButton		=	GLTexture::getTexture(L"images/ButtonOption.png");
	GLTexture::ptr CreditsButton	=	GLTexture::getTexture(L"images/ButtonCredits.png");
	GLTexture::ptr ExitButton		=	GLTexture::getTexture(L"images/ButtonExit.png");

	GLTexture::ptr LobbyButtonT		=	GLTexture::getTexture(L"images/ButtonLobbyT.png");
	GLTexture::ptr HtPButtonT		=	GLTexture::getTexture(L"images/ButtonHtPT.png");
	GLTexture::ptr OptionButtonT	=	GLTexture::getTexture(L"images/ButtonOptionT.png");
	GLTexture::ptr CreditsButtonT	=	GLTexture::getTexture(L"images/ButtonCreditsT.png");
	GLTexture::ptr ExitButtonT		=	GLTexture::getTexture(L"images/ButtonExitT.png");

	//class	 name	unpressed		pressed			rectangle	lower left corner		upper right corner
	Button button0(LobbyButton,		LobbyButtonT,	Rectanglef(glm::vec2(0.30f,0.25f),glm::vec2(0.40f,0.15f)), 0.0f);
	Button button1(HtPButton,		HtPButtonT,		Rectanglef(glm::vec2(0.53f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f);
	Button button2(OptionButton,	OptionButtonT,	Rectanglef(glm::vec2(0.27f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f);
	Button button3(CreditsButton,	CreditsButtonT,	Rectanglef(glm::vec2(0.80f,0.04f),glm::vec2(0.15f,0.10f)), 0.0f);
	Button button4(ExitButton,		ExitButtonT,	Rectanglef(glm::vec2(0.05f,0.04f),glm::vec2(0.15f,0.10f)), 0.0f);


	buttons.push_back(button0);
	buttons.push_back(button1);
	buttons.push_back(button2);
	buttons.push_back(button3);
	buttons.push_back(button4);
}

void MainMeny::createBackground()
{
	GLTexture::ptr Background = GLTexture::getTexture(L"images/MotBbackground.png");

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)), 0.99f);

	Backgrounds.push_back(Background1);
}

void MainMeny::KeyboardEventMethod(KeyboardEvent* keyEvent)
{
	const static char ESC = 0x1B;
	if (keyEvent->key == ESC && keyEvent->eventType == KeyboardEventType::Pressed)
	{
		game->close();
	}
	else if (keyEvent->key == 'c' && keyEvent->eventType == KeyboardEventType::Released)
	{
		game->connect();
	}
	else if (keyEvent->key == 'b' && keyEvent->eventType == KeyboardEventType::Released)
	{
		game->sendBlob();
	}
}

void MainMeny::MousePressEventMethod(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (buttons[0].getState() == Hovered)
		{
			nextScreen = Screen::ptr(new GameScreen());
		}

		if (buttons[1].getState() == Hovered)
		{
			nextScreen = Screen::ptr(new GameScreen());
		}

		if (buttons[2].getState() == Hovered)
		{
			nextScreen = Screen::ptr(new GameScreen());
		}

		if (buttons[3].getState() == Hovered)
		{
			nextScreen = Screen::ptr(new GameScreen());
		}

		if (buttons[4].getState() == Hovered)
		{
			game->close();
		}
	}
}

void MainMeny::MouseTouchEventMethod(MouseMoveEvent* mmEvent)
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