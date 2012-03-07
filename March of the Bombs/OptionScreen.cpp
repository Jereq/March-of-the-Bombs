#include "OptionScreen.h"
#include "Game.h"
#include "MainMeny.h"

OptionScreen::OptionScreen()
	: game(Game::getInstance())
{
	OptionScreen::createBackground();
	OptionScreen::createButtons();
}


OptionScreen::~OptionScreen()
{
}

void OptionScreen::update(float deltaTime)
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

void OptionScreen::draw(Graphics::ptr graphics)
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

Screen::ptr OptionScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void OptionScreen::createButtons()
{
	GLTexture::ptr BackButton		=	GLTexture::getTexture(L"images/NewBI/BackBtn1.png");
	
	GLTexture::ptr BackButtonT		=	GLTexture::getTexture(L"images/NewBI/BackBtn2.png");

	Button button0(BackButton,		BackButtonT,	Rectanglef(glm::vec2(0.40f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f);

	buttons.push_back(button0);

}

void OptionScreen::createBackground()
{
	GLTexture::ptr Background = GLTexture::getTexture(L"images/NewBI/Options.png");

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)), 0.99f);

	Backgrounds.push_back(Background1);
}

void OptionScreen::KeyboardEventMethod(KeyboardEvent* keyEvent)
{
	const static char ESC = 0x1B;
	if (keyEvent->key == ESC && keyEvent->eventType == KeyboardEventType::Pressed)
	{
		game->close();
	}
}

void OptionScreen::MousePressEventMethod(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (buttons[0].getState() == Hovered)
		{
			nextScreen = Screen::ptr(new MainMeny());
		}
	}
}

void OptionScreen::MouseTouchEventMethod(MouseMoveEvent* mmEvent)
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