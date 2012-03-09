#pragma once

#include "Screen.h"
#include "KeyboardEvent.h"
#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "GameClient.h"

class Game;

class LobbyScreen :
	public Screen
{
private:
	std::vector<Button::ptr> buttons;
	Button::ptr backButton;
	Button::ptr networkPlayButton;
	Button::ptr joinGameButton;
	Button::ptr createGameButton;
	Button::ptr signInButton;

	std::vector<SimpleImage> Backgrounds;
	Screen::ptr nextScreen;
	boost::shared_ptr<Game> game;
	GameClient::ptr client;

	std::string playerName;
	unsigned short playerID;

public:
	LobbyScreen();
	~LobbyScreen();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void createButtons();
	void createBackground();
	void KeyboardEventMethod(KeyboardEvent* keyEvent);
	void MousePressEventMethod(MouseButtonEvent* mbEvent);
	void MouseTouchEventMethod(MouseMoveEvent* mmEvent);

	virtual void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet);
	virtual void handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet);
};

