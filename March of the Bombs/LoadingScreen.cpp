#include "LoadingScreen.h"

#include "Game.h"
#include "GameScreen.h"

void LoadingScreen::loadGame()
{
	newGame = Screen::ptr(new GameScreen(client, playerName, playerID, playerColor,
		opponentName, opponentID, opponentColor, mapName, baseID));
}

LoadingScreen::LoadingScreen(GameClient::ptr const& client, std::string const& playerName, unsigned short playerID, glm::vec3 const& playerColor,
		std::string const& opponentName, unsigned short opponentID, glm::vec3 const& opponentColor,
		std::string const& mapName, unsigned short baseID)
	: client(client), playerName(playerName), playerID(playerID), playerColor(playerColor),
	  opponentName(opponentName), opponentID(opponentID), opponentColor(opponentColor),
	  mapName(mapName), baseID(baseID), opponentReady(false), ready(false)
{
}

void LoadingScreen::atEntry()
{
	Game::getInstance()->getGraphics()->setBackground(
		TextureSection::ptr(new TextureSection(L"Images/background_loading.png")));

	loadThread = boost::thread(boost::bind(&LoadingScreen::loadGame, this));
}

void LoadingScreen::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets() && !nextScreen)
		{
			Packet::ptr packet = client->popReceivedPacket();
			packet->dispatch(&shared_from_this());
		}
	}

	if (!ready)
	{
		if (loadThread.timed_join(boost::posix_time::milliseconds(0)))
		{
			Packet::ptr packetReady(new Packet10PlayerReady(playerID));
			client->write(packetReady);

			ready = true;
		}
	}

	if (ready && opponentReady)
	{
		nextScreen = newGame;
	}

	Game::getInstance()->getEvents().clear();
}

void LoadingScreen::draw(Graphics::ptr graphics)
{
}

Screen::ptr LoadingScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void LoadingScreen::handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet)
{
	opponentReady = true;
}