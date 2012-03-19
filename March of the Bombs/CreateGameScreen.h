#pragma once

#include "Screen.h"

#include "GameClient.h"
#include "MouseButtonEvent.h"
#include "MouseMoveEvent.h"
#include "Label.h"

class CreateGameScreen :
	public Screen
{
private:
	Screen::ptr previousScreen;
	Screen::ptr nextScreen;

	GameClient::ptr client;

	Button::ptr backButton;
	Button::ptr okButton;

	Label::ptr winLabel;
	TextField::ptr winLimit;
	SelectionList::ptr mapList;

	std::string playerName;
	unsigned short playerID;

	void goBack();

	std::vector<std::string> mapNames;

protected:
	void loadMapNames();

	void setupComponents();

	void keyboardEventHandler(KeyboardEvent* keyEvent);
	void mouseButtonEventHandler(MouseButtonEvent* mbEvent);
	void mouseMoveEventHandler(MouseMoveEvent* mmEvent);

public:
	CreateGameScreen(Screen::ptr const& parentScreen, GameClient::ptr const& client, std::string const& playerName, unsigned short playerID);

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();
};