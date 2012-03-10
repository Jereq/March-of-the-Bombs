#include "GUIMain.h"
#include "MainMeny.h"

GUIMain::GUIMain()
{
	screen = Screen::ptr(new MainMeny());				//creating a smartpointer attribute, allso the thing that will say what screen to use
}

GUIMain::~GUIMain()
{
}

//the final updatefunktion that uses the updateinfo from the currently pointing screen
void GUIMain::update(float deltaTime)
{
	//add some form of cheeck when to chance screen.. some funktion/bool or something simular
	screen->update(deltaTime);								//Pointing this function to use the pointers targets update 

	Screen::ptr next = screen->getNextScreen();
	while (next)
	{
		screen = next;

		screen->atEntry();

		next = screen->getNextScreen();
	}
}

//the final drawfunktion that uses the drawinfo from the currently pointing screen
void GUIMain::draw(Graphics::ptr graphics)
{
	screen->draw(graphics);								//Pointing this funktion to use the pointers targets draw
}
	
void GUIMain::handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet)
{
	screen->handlePacket1SimpleMessage(packet);
}

void GUIMain::handlePacket2Blob(Packet2Blob::const_ptr const& packet)
{
	screen->handlePacket2Blob(packet);
}

void GUIMain::handlePacket3Login(Packet3Login::const_ptr const& packet)
{
	screen->handlePacket3Login(packet);
}

void GUIMain::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet)
{
	screen->handlePacket4LoginAccepted(packet);
}

void GUIMain::handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet)
{
	screen->handlePacket5EntityMove(packet);
}

void GUIMain::handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet)
{
	screen->handlePacket6CreateGame(packet);
}

void GUIMain::handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet)
{
	screen->handlePacket7JoinGame(packet);
}

void GUIMain::handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet)
{
	screen->handlePacket8SetupGame(packet);
}

void GUIMain::handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet)
{
	screen->handlePacket9SpawnBomb(packet);
}

void GUIMain::handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet)
{
	screen->handlePacket10PlayerReady(packet);
}

void GUIMain::handlePacket11RequestOpenGames(Packet11RequestOpenGames::const_ptr const& packet)
{
	screen->handlePacket11RequestOpenGames(packet);
}

void GUIMain::handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet)
{
	screen->handlePacket12OpenGames(packet);
}