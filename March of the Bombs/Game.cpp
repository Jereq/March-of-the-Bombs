#include "Game.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#ifndef IL_USE_PRAGMA_LIBS
	#define IL_USE_PRAGMA_LIBS
#endif
#include <IL/ilut.h>

#include <boost/bind.hpp>

#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "MouseMoveEvent.h"

#include <Packet2Blob.h>

Game::ptr Game::instance = Game::ptr();

std::string Game::windowTitle = "March of the Bombs";

void Game::initOpenGL()
{
	int argc = 1;
	char* foo = "ProgramName";
	glutInit(&argc, &foo);

	int windowWidth = 1280;
	int windowHeight = 700;
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(windowTitle.c_str());
	
	glutDisplayFunc(Game::stDisplayFunc);
	glutReshapeFunc(Game::stReshapeFunc);
	glutMouseFunc(Game::stMouseFunc);
	glutMotionFunc(Game::stMouseMotionFunc);
	glutPassiveMotionFunc(Game::stMouseMotionFunc);
	glutKeyboardFunc(Game::stKeyDownFunc);
	glutKeyboardUpFunc(Game::stKeyUpFunc);

	glutIgnoreKeyRepeat(true);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("OpenGL on %s %s\n", vendor, renderer);
	printf("OpenGL version supported: %s\n", version);
	printf("OpenGL version supported: %i.%i\n", major, minor);
	printf("GLSL version supported: %s\n", glslVersion);

	// Version used
	major = 3;
	minor = 2;

	printf("Will now set GL version to: %i.%i\n", major, minor);
	glutInitContextVersion(major, minor);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW: %s\n",
			glewGetErrorString(err));
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glClearColor(0.5f, 0.5f, 0.5f, 1);
}

void Game::initDevIL()
{
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
}

void Game::stDisplayFunc()
{
	getInstance()->displayFunc();
}

void Game::displayFunc()
{
	gui->draw(graphics);

	graphics->render();

	glutSwapBuffers();

	return;
}

void Game::stReshapeFunc(int width, int height)
{
	getInstance()->reshapeFunc(width, height);
}

void Game::reshapeFunc(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	graphics->updateViewport();

	return;
}

void Game::stUpdateFunc(int value)
{
	getInstance()->updateFunc(value);
}

void Game::updateFunc(int value)
{
	if (value != validUpdate)
		return;


	int runTimeMS = glutGet(GLUT_ELAPSED_TIME);
	int deltaTimeMS = runTimeMS - previousTime;
	float deltaTime = deltaTimeMS / 1000.f;
	previousTime = runTimeMS;

	if (deltaTime > 1.f / fpsCap * 10)
	{
		deltaTime = 1.f / fpsCap * 10;
	}

	int mouseDeltaX = currentMouseX - previousMouseX;
	int mouseDeltaY = currentMouseY - previousMouseY;
	previousMouseX = currentMouseX;
	previousMouseY = currentMouseY;


	update(deltaTime);


	glutPostRedisplay();

	int updateTimeMS = glutGet(GLUT_ELAPSED_TIME) - runTimeMS;
	int delayMS = static_cast<int>(1000.f / fpsCap);

	int waitTimeMS;
	if (updateTimeMS > delayMS)
	{
		// TODO: signal slow program execution
		waitTimeMS = 0;
	}
	else
	{
		waitTimeMS = delayMS - updateTimeMS;
	}

	glutTimerFunc(waitTimeMS, stUpdateFunc, validUpdate);

	return;
}

void Game::stMouseFunc(int button, int state, int x, int y)
{
	getInstance()->mouseFunc(button, state, x, y);
}

void Game::mouseFunc(int button, int state, int x, int y)
{
	currentMouseX = x;
	currentMouseY = y;

	MouseButtonEvent::mouseButton eButton;
	MouseButtonEvent::mouseButtonState eState;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		eButton = MouseButton::Left;
		break;

	case GLUT_RIGHT_BUTTON:
		eButton = MouseButton::Right;
		break;

	case GLUT_MIDDLE_BUTTON:
		eButton = MouseButton::Middle;
		break;

	default:
		return;
	}

	switch (state)
	{
	case GLUT_DOWN:
		eState = MouseButtonState::Pressed;
		break;

	case GLUT_UP:
		eState = MouseButtonState::Released;
		break;

	default:
		return;
	}

	events.push_back(Event::ptr(new MouseButtonEvent(eButton, eState, getMousePos())));
}

void Game::stMouseMotionFunc(int x, int y)
{
	getInstance()->mouseMotionFunc(x, y, true);
}

void Game::stPassiveMouseMotionFunc(int x, int y)
{
	getInstance()->mouseMotionFunc(x, y, false);
}

void Game::mouseMotionFunc(int x, int y, bool pressed)
{
	currentMouseX = x;
	currentMouseY = y;

	events.push_back(Event::ptr(new MouseMoveEvent(getMousePos(), pressed)));
}

void Game::stKeyDownFunc(unsigned char key, int x, int y)
{
	getInstance()->keyDownFunc(key, x, y);
}

void Game::keyDownFunc(unsigned char key, int x, int y)
{
	events.push_back(Event::ptr(new KeyboardEvent(key, KeyboardEventType::Pressed)));
}

void Game::stKeyUpFunc(unsigned char key, int x, int y)
{
	getInstance()->keyUpFunc(key, x, y);
}

void Game::keyUpFunc(unsigned char key, int x, int y)
{
	events.push_back(Event::ptr(new KeyboardEvent(key, KeyboardEventType::Released)));
}

void Game::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets())
		{
			Packet::ptr packet = client->popReceivedPacket();
			packet->dispatch(NULL);
		}
	}

	gui->update(deltaTime);
}

Game::Game()
	: fpsCap(60.f), paused(true), validUpdate(0), previousTime(glutGet(GLUT_ELAPSED_TIME))
{
	graphics = Graphics::ptr(new Graphics());
}

Game::~Game()
{
}

void Game::start()
{
	if (!gui)
	{
		gui = GUIMain::ptr(new GUIMain());
	}

	unpause();
	glutMainLoop();
}

Game::ptr Game::getInstance()
{
	if (!instance)
	{
		initOpenGL();
		initDevIL();
		instance = ptr(new Game());
	}

	return instance;
}

void Game::pause()
{
	if (!paused)
	{
		validUpdate++;

		paused = true;
	}
}

void Game::unpause()
{
	if (paused)
	{
		previousTime = glutGet(GLUT_ELAPSED_TIME);
		glutTimerFunc(0, stUpdateFunc, validUpdate);

		paused = false;
	}
}

void Game::setPaused(bool pauseGame)
{
	if (pauseGame)
	{
		pause();
	}
	else
	{
		unpause();
	}
}

bool Game::isPaused() const
{
	return paused;
}

void Game::setFpsCap(float cap)
{
	fpsCap = cap;
}

float Game::getFpsCap() const
{
	return fpsCap;
}

glm::vec2 Game::getMousePos() const
{
	return glm::vec2(static_cast<float>(currentMouseX) / windowWidth, 1.f - static_cast<float>(currentMouseY) / windowHeight);
}

std::deque<Event::ptr>& Game::getEvents()
{
	return events;
}

void Game::close()
{
	glutLeaveMainLoop();
}

void Game::connect()
{
	if (client)
	{
		client.reset();
		ioService.reset();
	}

	using boost::asio::ip::tcp;

	tcp::resolver resolver(ioService);
	tcp::resolver::query query("localhost", "1694");
	tcp::resolver::iterator endpointIterator = resolver.resolve(query);

	client = GameClient::ptr(new GameClient(ioService, endpointIterator));
	client->start();
}

void Game::sendBlob()
{
	if (client)
	{
		char data[Packet::MAX_LOAD_SIZE];
		Packet::ptr packet = Packet::ptr(new Packet2Blob(data, Packet::MAX_LOAD_SIZE));

		client->write(packet);
	}
}

int Game::getWindowWidth() const
{
	return windowWidth;
}

int Game::getWindowHeight() const
{
	return windowHeight;
}

Graphics::ptr const& Game::getGraphics() const
{
	return graphics;
}