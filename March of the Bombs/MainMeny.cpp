#include "MainMeny.h"
#include "Game.h"
#include <boost/foreach.hpp>
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "GameScreen.h"

MainMeny::MainMeny()
{
	MainMeny::createBackground();				//creates the vector holding all the backgrounds for this screen
	MainMeny::createButtons();					//creates the vector holding all the buttons for this screen
}

MainMeny::~MainMeny()
{
}

//Mainmenys updateinfo
void MainMeny::update()
{
	Game::ptr game = Game::getInstance();

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
			break;

		case EventType::MouseButton:
			{
				MouseButtonEvent* mbEvent = static_cast<MouseButtonEvent*>(ev.get());

				if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
				{
					BOOST_FOREACH(Button& button, buttons)
					{
						if(button.intersects(mbEvent->position))
						{
							button.changeState();
						}
					}

					if (buttons[0].isPressed())
					{
						nextScreen = Screen::ptr(new GameScreen());
					}

					if (buttons[4].isPressed())
					{
						game->close();
					}
				}
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
	//GLTexture::ptr TestButton1 =	GLTexture::loadTexture(L"images/skull.tga");
	GLTexture::ptr LobbyButton =	GLTexture::loadTexture(L"images/ButtonLobby.png");
	GLTexture::ptr HtPButton =		GLTexture::loadTexture(L"images/ButtonHtP.png");
	GLTexture::ptr OptionButton =	GLTexture::loadTexture(L"images/ButtonOption.png");
	GLTexture::ptr CreditsButton =	GLTexture::loadTexture(L"images/ButtonCredits.png");
	GLTexture::ptr ExitButton =		GLTexture::loadTexture(L"images/ButtonExit.png");

	//class	 name	unpressed		pressed			rectangle	lower left corner		upper right corner
	Button button0(LobbyButton,		LobbyButton,	Rectanglef(glm::vec2(0.25f,0.50f),glm::vec2(0.50f,0.15f)), 0.0f);
	Button button1(HtPButton,		HtPButton,		Rectanglef(glm::vec2(0.30f,0.39f),glm::vec2(0.40f,0.10f)), 0.0f);
	Button button2(OptionButton,	OptionButton,	Rectanglef(glm::vec2(0.33f,0.26f),glm::vec2(0.35f,0.10f)), 0.0f);
	Button button3(CreditsButton,	CreditsButton,	Rectanglef(glm::vec2(0.36f,0.14f),glm::vec2(0.30f,0.10f)), 0.0f);
	Button button4(ExitButton,		ExitButton,		Rectanglef(glm::vec2(0.38f,0.02f),glm::vec2(0.25f,0.08f)), 0.0f);


	buttons.push_back(button0);
	buttons.push_back(button1);
	buttons.push_back(button2);
	buttons.push_back(button3);
	buttons.push_back(button4);
}

void MainMeny::createBackground()
{
	GLTexture::ptr Background = GLTexture::loadTexture(L"images/MotBbackground.png");

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)), 0.99f);

	Backgrounds.push_back(Background1);
}