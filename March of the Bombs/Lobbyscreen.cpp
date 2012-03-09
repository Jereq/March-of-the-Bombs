#include "LobbyScreen.h"
#include "Game.h"
#include "MainMeny.h"
#include "GameScreen.h"

LobbyScreen::LobbyScreen()
	: game(Game::getInstance())
{
	LobbyScreen::createBackground();
	LobbyScreen::createButtons();
}


LobbyScreen::~LobbyScreen()
{
}

void LobbyScreen::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets())
		{
			Packet::ptr packet = client->popReceivedPacket();
			packet->dispatch(&shared_from_this());
		}
	}

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

void LobbyScreen::draw(Graphics::ptr graphics)
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

Screen::ptr LobbyScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void LobbyScreen::createButtons()
{
	TextureSection BackButton(L"images/NewBI/BackBtn1.png");
	TextureSection NPButton(L"images/NewBI/NPBtn1.png");
	TextureSection JGButton(L"images/NewBI/JGBtn1.png");
	TextureSection CGButton(L"images/NewBI/CGBtn1.png");
	TextureSection SInButton(L"images/NewBI/SignInBtn1.png");

	TextureSection BackButtonT(L"images/NewBI/BackBtn2.png");
	TextureSection NPButtonT(L"images/NewBI/NPBtn2.png");
	TextureSection JGButtonT(L"images/NewBI/JGBtn2.png");
	TextureSection CGButtonT(L"images/NewBI/CGBtn2.png");
	TextureSection SInButtonT(L"images/NewBI/SignInBtn2.png");

	Button button0(BackButton,		BackButtonT,	Rectanglef(glm::vec2(0.40f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f);
	Button button1(NPButton,		NPButtonT,		Rectanglef(glm::vec2(0.40f,0.20f),glm::vec2(0.20f,0.10f)), 0.0f);
	Button button2(JGButton,		JGButtonT,		Rectanglef(glm::vec2(0.10f,0.10f),glm::vec2(0.20f,0.10f)), 0.0f);
	Button button3(CGButton,		CGButtonT,		Rectanglef(glm::vec2(0.10f,0.30f),glm::vec2(0.20f,0.10f)), 0.0f);
	Button button4(SInButton,		SInButtonT,		Rectanglef(glm::vec2(0.10f,0.50f),glm::vec2(0.20f,0.10f)), 0.0f);


	buttons.push_back(button0);
	buttons.push_back(button1);
	buttons.push_back(button2);
	buttons.push_back(button3);
	buttons.push_back(button4);
}

void LobbyScreen::createBackground()
{
	TextureSection Background(L"images/NewBI/Start.png");

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)), 0.99f);

	Backgrounds.push_back(Background1);
}

void LobbyScreen::KeyboardEventMethod(KeyboardEvent* keyEvent)
{
	const static char ESC = 0x1B;

	if (keyEvent->eventType == KeyboardEventType::Pressed)
	{
		switch (keyEvent->key)
		{
		case ESC:
			game->close();
			break;

		case '0':
			{
				client.reset();
				client.reset(new GameClient("localhost", "1694"));

				playerName = "foo";
				Packet::ptr packet = Packet::ptr(new Packet3Login(playerName));
				client->write(packet);
			}
			break;

		case '1':
			{
				client.reset();
				client.reset(new GameClient("192.168.1.20", "1694"));

				playerName = "bar";
				Packet::ptr packet = Packet::ptr(new Packet3Login(playerName));
				client->write(packet);
			}
			break;

		case 'v':
			if (client)
			{
				Packet::ptr packet = Packet::ptr(new Packet6CreateGame("defaultmapfile"));
				client->write(packet);
			}
			break;

		case 'b':
			if (client)
			{
				Packet::ptr packet = Packet::ptr(new Packet7JoinGame(0));
				client->write(packet);
			}
			break;
		}
	}
}

void LobbyScreen::MousePressEventMethod(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (buttons[0].getState() == Hovered)
		{
			nextScreen = Screen::ptr(new MainMeny());
			game->getEvents().clear();
		}
	}
}

void LobbyScreen::MouseTouchEventMethod(MouseMoveEvent* mmEvent)
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

void LobbyScreen::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet)
{
	Packet4LoginAccepted const* packet4 = static_cast<Packet4LoginAccepted const*>(packet.get());

	playerID = packet4->getPlayerID();
}

void LobbyScreen::handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet)
{
	Packet8SetupGame const* packet8 = static_cast<Packet8SetupGame const*>(packet.get());

	nextScreen = Screen::ptr(new GameScreen(client, packet8->getMapName(), playerID,
		packet8->getOpponentID(), packet8->getBaseID(), packet8->getOpponentColor()));
	game->getEvents().clear();
}