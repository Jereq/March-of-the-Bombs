#pragma once

#include "Screen.h"
#include "Label.h"
#include "MouseMoveEvent.h"

class GameOverScreen :
	public Screen
{
private:
	Screen::ptr nextScreen;

	Button::ptr mainMenyButton;
	Label::ptr result;
	Label::ptr winningPlayer;
	Label::ptr losingPlayer;
	Label::ptr winScore;
	Label::ptr loseScore;

	std::string myName;
	std::string opponentName;
	unsigned int myScore;
	unsigned int opponentScore;

	void goBack();

public:
	GameOverScreen(std::string const& myName, std::string const& opponentName,
		unsigned int myScore, unsigned int opponentScore);

	void atEntry();

	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void keyboardEventHandler(KeyboardEvent* keyEvent);
	void mouseButtonEventHandler(MouseButtonEvent* mbEvent);
	void mouseMoveEventHandler(MouseMoveEvent* mmEvent);
};