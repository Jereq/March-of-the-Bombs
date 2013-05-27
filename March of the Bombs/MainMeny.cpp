#include "MainMeny.h"
#include "Game.h"
#include "LoadingScreen.h"

MainMeny::MainMeny()
	: game(Game::getInstance())
{
	MainMeny::createBackground();
	MainMeny::createButtons();			//creates the vector holding all the buttons for this screen
	labels.push_back(Label::ptr(new Label(glm::vec2(0.05f, 0.8f), glm::vec2(0.05f, 0.12f), "March of the Bombs", 0.f, glm::vec3(1.f))));
	labels.push_back(Label::ptr(new Label(glm::vec2(0.30f, 0.65f), glm::vec2(0.025f, 0.06f), "Sound Downgrade", 0.f, glm::vec3(1.f, 1.f, 0.8f))));
	labels.push_back(Label::ptr(new Label(glm::vec2(0.45f, 0.1f), glm::vec2(0.0125f, 0.03f), "By: Fredrik Cronqvist, Sebastian Larsson", 0.f, glm::vec3(1.f, 1.f, 0.8f))));
}

MainMeny::~MainMeny()
{
}

void MainMeny::atEntry()
{
	game->getSoundManager()->playBackgroundSound("Sounds/Mainbackground.mp3", 0.1f);
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

	for (Label::ptr const& lable : labels)
	{
		lable->draw(graphics);
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
	TextureSection LobbyButton		(L"images/NewBI/StartUp.png");
	TextureSection ExitButton		(L"images/NewBI/ExitBtn1.png");

	TextureSection LobbyButtonT		(L"images/NewBI/StartDown.png");
	TextureSection ExitButtonT		(L"images/NewBI/ExitBtn2.png");

	lobbyButton		= Button::ptr(new Button(LobbyButton,		LobbyButtonT,	Rectanglef(glm::vec2(0.30f,0.25f),glm::vec2(0.40f,0.15f)), 0.0f));
	exitButton		= Button::ptr(new Button(ExitButton,		ExitButtonT,	Rectanglef(glm::vec2(0.05f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f));

	buttons.push_back(lobbyButton);
	buttons.push_back(exitButton);
}

void MainMeny::createBackground()
{
	TextureSection::ptr background(new TextureSection(L"Images/background_meny.png"));
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
			nextScreen.reset(new LoadingScreen());

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