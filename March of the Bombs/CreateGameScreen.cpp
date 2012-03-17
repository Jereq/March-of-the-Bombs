#include "CreateGameScreen.h"

#include "Game.h"

#include "GameScreen.h"
#include "LoadingScreen.h"

void CreateGameScreen::goBack()
{
	Game::getInstance()->getEvents().clear();
	nextScreen = previousScreen;
}

void CreateGameScreen::setupComponents()
{
	backButton.reset(new Button(
		TextureSection(L"Images/NewBI/BackBtn1.png"),
		TextureSection(L"Images/NewBI/BackBtn2.png"),
		Rectanglef(glm::vec2(0.2f, 0.1f), glm::vec2(0.25f, 0.1f)),
		0));

	okButton.reset(new Button(
		TextureSection(L"Images/NewBI/NPBtn1.png"),
		TextureSection(L"Images/NewBI/NPBtn2.png"),
		Rectanglef(glm::vec2(0.55f, 0.1f), glm::vec2(0.25f, 0.1f)),
		0));

	mapName.reset(new TextField(
		TextureSection(L"Images/TFBackground.png"),
		TextureSection(L"Images/DTFBackground.png"),
		Rectanglef(glm::vec2(0.55f, 0.7f), glm::vec2(0.35f, 0.05f)),
		0));
}

void CreateGameScreen::keyboardEventHandler(KeyboardEvent* keyEvent)
{
	const static char ESC = 0x1B;

	if (keyEvent->keyEventType == KeyboardEventType::Pressed)
	{
		switch (keyEvent->key)
		{
		case ESC:
			goBack();
			break;

		default:
			mapName->updateString(keyEvent);
			break;
		}
	}
}

void CreateGameScreen::mouseButtonEventHandler(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (backButton->getState() == Hovered)
		{
			goBack();
		}
		else if (okButton->getState() == Hovered)
		{
			if (client)
			{
				Packet::ptr packet = Packet::ptr(new Packet6CreateGame(mapName->getString()));
				client->write(packet);
			}
		}
		
		mapName->active = (mapName->getState() == Targeted);
	}
}

void CreateGameScreen::mouseMoveEventHandler(MouseMoveEvent* mmEvent)
{
	if (backButton->intersects(mmEvent->position))
	{
		backButton->setState(Hovered);
	}
	else
	{
		backButton->setState(Unused);
	}

	if (okButton->intersects(mmEvent->position))
	{
		okButton->setState(Hovered);
	}
	else
	{
		okButton->setState(Unused);
	}

	if(mapName->intersects(mmEvent->position))
	{
		mapName->setState(Targeted);
	}
	else
	{
		mapName->setState(NotTargeted);
	}
}

CreateGameScreen::CreateGameScreen(Screen::ptr const& parentScreen, GameClient::ptr const& client,
		std::string const& playerName, unsigned short playerID)
	: previousScreen(parentScreen), client(client),
	  playerName(playerName), playerID(playerID)
{
	setupComponents();
}

void CreateGameScreen::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets() && !nextScreen)
		{
			Packet::ptr packet = client->popReceivedPacket();
			packet->dispatch(&shared_from_this());
		}
	}

	Game::ptr& game(Game::getInstance());

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

void CreateGameScreen::draw(Graphics::ptr graphics)
{
	backButton->render(graphics);
	okButton->render(graphics);

	mapName->render(graphics);
}

Screen::ptr CreateGameScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void CreateGameScreen::handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet)
{
	Packet8SetupGame const* packet8 = static_cast<Packet8SetupGame const*>(packet.get());

	glm::vec3 myColor = glm::vec3(1.f, 1.f, 0.f) - packet8->getOpponentColor();
	
	nextScreen.reset(new LoadingScreen(client, playerName, playerID, myColor,
		"Hasse", packet8->getOpponentID(), packet8->getOpponentColor(),
		packet8->getMapName(), packet8->getBaseID()));
	Game::getInstance()->getEvents().clear();
}