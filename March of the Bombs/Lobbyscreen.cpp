#include "LobbyScreen.h"
#include "Game.h"
#include "MainMeny.h"
#include "GameScreen.h"

LobbyScreen::LobbyScreen()
	: game(Game::getInstance())
{
	createBackground();
	createButtons();
	createTextFields();
}

LobbyScreen::~LobbyScreen()
{
}

void LobbyScreen::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets() && !nextScreen)
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
		buttons[i]->render(graphics);
	}

	//starts to render all the TextFields
	for(unsigned int i = 0; i < textfields.size(); i++)
	{
		textfields[i]->render(graphics);
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

	backButton			= Button::ptr(new Button(BackButton,	BackButtonT,	Rectanglef(glm::vec2(0.40f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f));
	networkPlayButton	= Button::ptr(new Button(NPButton,		NPButtonT,		Rectanglef(glm::vec2(0.70f,0.04f),glm::vec2(0.20f,0.10f)), 0.0f));
	joinGameButton		= Button::ptr(new Button(JGButton,		JGButtonT,		Rectanglef(glm::vec2(0.05f,0.20f),glm::vec2(0.20f,0.10f)), 0.0f));
	createGameButton	= Button::ptr(new Button(CGButton,		CGButtonT,		Rectanglef(glm::vec2(0.05f,0.40f),glm::vec2(0.20f,0.10f)), 0.0f));
	signInButton		= Button::ptr(new Button(SInButton,		SInButtonT,		Rectanglef(glm::vec2(0.05f,0.60f),glm::vec2(0.20f,0.10f)), 0.0f));
	
	buttons.push_back(backButton);
	buttons.push_back(networkPlayButton);
	buttons.push_back(joinGameButton);
	buttons.push_back(createGameButton);
	buttons.push_back(signInButton);
}

void LobbyScreen::createBackground()
{
	TextureSection Background(L"images/NewBI/Start.png");

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)), 0.99f);

	Backgrounds.push_back(Background1);
}

void LobbyScreen::createTextFields()
{
	TextureSection TFBackground(L"Images/TFBackground.png");

	SignInTF	= TextField::ptr(new TextField(TFBackground,Rectanglef(glm::vec2(0.30f,0.60f),glm::vec2(0.40f,0.10f)),0.0f));
	IPTF		= TextField::ptr(new TextField(TFBackground,Rectanglef(glm::vec2(0.30f,0.20f),glm::vec2(0.40f,0.10f)),0.0f));

	textfields.push_back(SignInTF);
	textfields.push_back(IPTF);
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
		}
	}
}

void LobbyScreen::MousePressEventMethod(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (backButton->getState() == Hovered)
		{
			nextScreen = Screen::ptr(new MainMeny());
			game->getEvents().clear();
		}
		else if (networkPlayButton->getState() == Hovered)
		{
			client.reset();
			client.reset(new GameClient("localhost", "1694"));

			playerName = "foo";
			Packet::ptr packet = Packet::ptr(new Packet3Login(playerName));
			client->write(packet);
		}
		else if (signInButton->getState() == Hovered)
		{
			client.reset();
			client.reset(new GameClient("192.168.1.20", "1694"));

			playerName = "bar";
			Packet::ptr packet = Packet::ptr(new Packet3Login(playerName));
			client->write(packet);
			signInButton->setState(Disable);
		}
		else if (createGameButton->getState() == Hovered)
		{
			if (client)
			{
				Packet::ptr packet = Packet::ptr(new Packet6CreateGame("defaultmapfile"));
				client->write(packet);
			}
		}
		else if (joinGameButton->getState() == Hovered)
		{
			if (client && !openGames.empty())
			{
				Packet::ptr packet = Packet::ptr(new Packet7JoinGame(openGames[0].getGameID()));
				client->write(packet);
			}
		}
	}
}

void LobbyScreen::MouseTouchEventMethod(MouseMoveEvent* mmEvent)
{
	BOOST_FOREACH(Button::ptr& button, buttons)
	{
		if(button->intersects(mmEvent->position) && button->getState() != Disable)
		{
			button->setState(Hovered);
		}
		else
		{
			button->setState(Unused);
		}
	}
}

void LobbyScreen::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet)
{
	Packet4LoginAccepted const* packet4 = static_cast<Packet4LoginAccepted const*>(packet.get());

	playerID = packet4->getPlayerID();

	Packet::ptr packet11(new Packet11RequestOpenGames());
	client->write(packet11);
}

void LobbyScreen::handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet)
{
	Packet8SetupGame const* packet8 = static_cast<Packet8SetupGame const*>(packet.get());

	newGame = Screen::ptr(new GameScreen(client, packet8->getMapName(), playerID,
		packet8->getOpponentID(), packet8->getBaseID(), packet8->getOpponentColor()));

	Packet::ptr packetReady(new Packet10PlayerReady(playerID));
	client->write(packetReady);
}

void LobbyScreen::handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet)
{
	nextScreen = newGame;
	game->getEvents().clear();
}

void LobbyScreen::handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet)
{
	Packet12OpenGames const* packet12 = static_cast<Packet12OpenGames const*>(packet.get());

	openGames = packet12->getOpenGames();
}