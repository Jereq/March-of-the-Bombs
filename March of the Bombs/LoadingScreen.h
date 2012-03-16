#pragma once

#include "Screen.h"
#include "GameClient.h"

class LoadingScreen
	: public Screen
{
private:
	Screen::ptr nextScreen;

	GameClient::ptr client;

	std::string playerName;
	unsigned short playerID;
	glm::vec3 playerColor;

	std::string opponentName;
	unsigned short opponentID;
	glm::vec3 opponentColor;

	std::string mapName;
	unsigned short baseID;

	Screen::ptr newGame;

public:
	LoadingScreen(GameClient::ptr const& client, std::string const& playerName, unsigned short playerID, glm::vec3 const& playerColor,
		std::string const& opponentName, unsigned short opponentID, glm::vec3 const& opponentColor,
		std::string const& mapName, unsigned short baseID);

	void atEntry();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	virtual void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet);
};