#pragma once

#include <string>
#include <deque>

#include <boost/shared_ptr.hpp>

#include "Camera.h"
#include "GUIMain.h"
#include "Event.h"
#include "GameClient.h"

class Game
{
public:
	typedef boost::shared_ptr<Game> ptr;

private:
	static std::string windowTitle;

	GUIMain gui;
	Graphics::ptr graphics;
	std::deque<Event::ptr> events;

	boost::asio::io_service ioService;
	GameClient::ptr client;

	bool paused;
	int validUpdate;
	int previousTime;
	float fpsCap;

	int previousMouseY;
	int previousMouseX;
	int currentMouseX;
	int currentMouseY;

	int windowWidth;
	int windowHeight;

	static void initOpenGL();
	static void initDevIL();

	static void stDisplayFunc();
	static void stReshapeFunc(int width, int height);
	static void stUpdateFunc(int value);
	static void stMouseFunc(int button, int state, int x, int y);
	static void stMouseMotionFunc(int x, int y);
	static void stPassiveMouseMotionFunc(int x, int y);
	static void stKeyDownFunc(unsigned char key, int x, int y);
	static void stKeyUpFunc(unsigned char key, int x, int y);

	void displayFunc();
	void reshapeFunc(int width, int height);
	void updateFunc(int value);
	void mouseFunc(int button, int state, int x, int y);
	void mouseMotionFunc(int x, int y, bool pressed);
	void keyDownFunc(unsigned char key, int x, int y);
	void keyUpFunc(unsigned char key, int x, int y);

	void update(float deltaTime);

	static ptr instance;
	Game();

public:
	~Game();

	void start();

	static ptr getInstance();
	
	void pause();
	void unpause();
	void setPaused(bool pause);
	bool isPaused() const;

	void setFpsCap(float cap);
	float getFpsCap() const;

	glm::vec2 getMousePos() const;
	std::deque<Event::ptr>& getEvents();

	void close();

	void connect();
	void sendBlob();

	int getWindowWidth() const;
	int getWindowHeight() const;

	Graphics::ptr const& getGraphics() const;
};