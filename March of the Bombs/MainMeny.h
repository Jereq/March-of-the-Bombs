#pragma once

#include "Screen.h"
#include "KeyboardEvent.h"
#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "Animation.h"

class Game;

class MainMeny :
	public Screen
{

private:
	//Button variables
	std::vector<Button::ptr> buttons;
	Button::ptr lobbyButton;
	Button::ptr htPButton;
	Button::ptr optionButton;
	Button::ptr creditsButton;
	Button::ptr exitButton;

	Screen::ptr nextScreen;
	boost::shared_ptr<Game> game;

public:
	MainMeny();
	~MainMeny();

	void atEntry();
	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void createButtons();
	void createBackground();
	void KeyboardEventMethod(KeyboardEvent* keyEvent);
	void MousePressEventMethod(MouseButtonEvent* mbEvent);
	void MouseTouchEventMethod(MouseMoveEvent* mmEvent);
};