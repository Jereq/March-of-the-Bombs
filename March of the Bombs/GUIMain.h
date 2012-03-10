#pragma once

#include "Screen.h"
#include "Graphics.h"

class GUIMain
{
private:
	Screen::ptr screen;						//smartpointer pointing at screen

public:
	typedef boost::shared_ptr<GUIMain> ptr;

	GUIMain();								//constructor
	virtual ~GUIMain();						//destructor
	void update(float deltaTime);			//updatefunction
	void draw(Graphics::ptr graphics);		//drawfunction
	
	void handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet);
	void handlePacket2Blob(Packet2Blob::const_ptr const& packet);
	void handlePacket3Login(Packet3Login::const_ptr const& packet);
	void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet);
	void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet);
	void handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet);
	void handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet);
	void handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet);
	void handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet);
	void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet);
	void handlePacket11RequestOpenGames(Packet11RequestOpenGames::const_ptr const& packet);
	void handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet);
};