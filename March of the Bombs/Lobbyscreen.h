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
	//Button variables
	std::vector<Button::ptr> buttons;
	Button::ptr backButton;
	Button::ptr refreshButton;
	Button::ptr joinGameButton;
	Button::ptr createGameButton;
	Button::ptr signInButton;

	//TextFeild

	TextField::ptr IPTF;
	TextField::ptr SignInTF;

	//SelectionList
	std::vector<SelectionList::ptr> SelectionLists;
	SelectionList::ptr selectionList;
	
	//Background
	std::vector<SimpleImage> Backgrounds;

	Screen::ptr nextScreen;
	boost::shared_ptr<Game> game;
	GameClient::ptr client;

	std::string playerName;
	unsigned short playerID;
	Screen::ptr newGame;

	std::vector<OpenGame> openGames;
	//std::vector<std::string> ListofGames;

public:
	LobbyScreen();
	~LobbyScreen();

	std::vector<TextField::ptr> textfields;

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	//createfunctions
	void createButtons();
	void createBackground();
	void createTextFields();
	void createListofGame();
	void converttoStringVector();

	void KeyboardEventMethod(KeyboardEvent* keyEvent);
	void MousePressEventMethod(MouseButtonEvent* mbEvent);
	void MouseTouchEventMethod(MouseMoveEvent* mmEvent);

	virtual void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet);
	virtual void handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet);
	virtual void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet);
	virtual void handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet);
};

