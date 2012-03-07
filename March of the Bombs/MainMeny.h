#pragma once

#include "Screen.h"
#include "KeyboardEvent.h"
#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"

class Game;

class MainMeny :
	public Screen
{

private:
	std::vector<Button> buttons;
	std::vector<SimpleImage> Backgrounds;
	Screen::ptr nextScreen;
	boost::shared_ptr<Game> game;

public:
	MainMeny();
	~MainMeny();
	void update(float deltaTime);
	void draw(Graphics::ptr graphics);
	Screen::ptr getNextScreen();

	void createButtons();
	void createBackground();
	void KeyboardEventMethod(KeyboardEvent* keyEvent);
	void MousePressEventMethod(MouseButtonEvent* mbEvent);
	void MouseTouchEventMethod(MouseMoveEvent* mmEvent);
	
	virtual void handlePacket2Blob(Packet2Blob::const_ptr const& packet);
	virtual void handlePacket4LoginAccepted(Packet2Blob::const_ptr const& packet);
};