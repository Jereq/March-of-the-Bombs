#include "CreateGameScreen.h"

#include <boost/filesystem.hpp>

#include "Game.h"

#include "GameScreen.h"
#include "LoadingScreen.h"

void CreateGameScreen::goBack()
{
	Game::getInstance()->getEvents().clear();
	nextScreen = previousScreen;
}

void CreateGameScreen::loadMapNames()
{
	using namespace boost::filesystem;

	path mapDirectory("Maps");
	assert(is_directory(mapDirectory));

	directory_iterator begin(mapDirectory);
	directory_iterator end;
	for(directory_iterator it = begin; it != end; ++it)
	{
		path const& p(it->path());

		if (is_regular_file(p) && p.extension() == ".txt")
		{
			mapNames.push_back(p.stem().generic_string());
		}
	}

	mapList->GameList = mapNames;
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

	mapList.reset(new SelectionList(
		TextureSection(L"Images/TFBackground.png"),
		Rectanglef(glm::vec2(0.1f, 0.3f), glm::vec2(0.8f, 0.6f)),
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
				int selectedMap = mapList->getindex();

				if (selectedMap >= 0 && static_cast<size_t>(selectedMap) < mapNames.size())
				{
					Packet::ptr packet = Packet::ptr(new Packet6CreateGame(mapNames[static_cast<size_t>(selectedMap)]));
					client->write(packet);
				}
			}
		}
		
		mapList->updateselection(mbEvent);
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
}

CreateGameScreen::CreateGameScreen(Screen::ptr const& parentScreen, GameClient::ptr const& client,
		std::string const& playerName, unsigned short playerID)
	: previousScreen(parentScreen), client(client),
	  playerName(playerName), playerID(playerID)
{
	setupComponents();
	loadMapNames();
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

	mapList->render(graphics);
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