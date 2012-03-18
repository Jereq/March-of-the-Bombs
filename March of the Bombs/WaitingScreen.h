#pragma once

#include "Screen.h"
#include "GameClient.h"

class WaitingScreen :
	public Screen
{
private:
	Screen::ptr nextScreen;

	GameClient::ptr client;

	std::string playerName;
	unsigned short playerID;

	std::string mapName;

public:
	typedef boost::shared_ptr<WaitingScreen> ptr;

	WaitingScreen(GameClient::ptr const& client, std::string const& playerName,
		unsigned short playerID, std::string const& mapName);

	void atEntry();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();
	
	virtual void handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet);
};