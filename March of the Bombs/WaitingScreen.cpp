#include "WaitingScreen.h"

#include "Game.h"
#include "LoadingScreen.h"

WaitingScreen::WaitingScreen(GameClient::ptr const& client, std::string const& playerName,
	unsigned short playerID, std::string const& mapName, unsigned short winLimit)
	: client(client), playerName(playerName), playerID(playerID),
	mapName(mapName), winLimit(winLimit)
{
}

void WaitingScreen::atEntry()
{
	Game::getInstance()->getGraphics()->setBackground(
		TextureSection::ptr(new TextureSection(L"Images/background_waiting.png")));

	Packet::ptr packet = Packet::ptr(new Packet6CreateGame(mapName, winLimit));
	client->write(packet);
}

void WaitingScreen::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets() && !nextScreen)
		{
			Packet::ptr packet = client->popReceivedPacket();
			packet->dispatch(&shared_from_this());
		}
	}

	Game::getInstance()->getEvents().clear();
}

void WaitingScreen::draw(Graphics::ptr graphics)
{
}

Screen::ptr WaitingScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void WaitingScreen::handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet)
{
	Packet8SetupGame const* packet8 = static_cast<Packet8SetupGame const*>(packet.get());

	glm::vec3 myColor = glm::vec3(1.f, 1.f, 0.f) - packet8->getOpponentColor();
	
	nextScreen.reset(new LoadingScreen(client, playerName, playerID, myColor,
		packet8->getOpponentName(), packet8->getOpponentID(), packet8->getOpponentColor(),
		packet8->getMapName(), packet8->getBaseID(), packet8->getWinLimit()));
}