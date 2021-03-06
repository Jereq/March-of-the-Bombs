#pragma once

#include <boost/thread.hpp>

#include "Screen.h"
#include "GameClient.h"
#include "GameScreen.h"
#include "Label.h"

class LoadingScreen
	: public Screen
{
private:
	boost::thread loadThread;

	Screen::ptr newGame;

	bool ready;
	bool opponentReady;

	Screen::ptr nextScreen;

	GameClient::ptr client;

	Label opponentLabel;

	std::string playerName;
	unsigned short playerID;
	glm::vec3 playerColor;

	std::string opponentName;
	unsigned short opponentID;
	glm::vec3 opponentColor;

	std::string mapName;
	unsigned short baseID;
	unsigned short winLimit;

	void loadGame();

public:
	typedef boost::shared_ptr<LoadingScreen> ptr;

	LoadingScreen(GameClient::ptr const& client, std::string const& playerName, unsigned short playerID, glm::vec3 const& playerColor,
		std::string const& opponentName, unsigned short opponentID, glm::vec3 const& opponentColor,
		std::string const& mapName, unsigned short baseID, unsigned short winLimit);

	void atEntry();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	virtual void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet);
};