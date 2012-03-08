#pragma once

#include <boost/enable_shared_from_this.hpp>							//include a boost smartpointer
#include <boost/foreach.hpp>
#include "Graphics.h"
#include "Button.h"
#include "SimpleImage.h"
#include "GLTexture.h"
#include <vector>

#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>
#include <Packet4LoginAccepted.h>
#include <Packet5EntityMove.h>
#include <Packet6CreateGame.h>
#include <Packet7JoinGame.h>

class Screen
	: public boost::enable_shared_from_this<Screen>
{
public:
	typedef boost::shared_ptr<Screen> ptr;				//A smartpointer

	virtual void update(float deltaTime) = 0;			//Makes the other classes aware of the function, here they are abstract and not used
	virtual void draw(Graphics::ptr graphics) = 0;		//Same as for update-function
	virtual ptr getNextScreen() = 0;
	
	virtual void handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet) {}
	virtual void handlePacket2Blob(Packet2Blob::const_ptr const& packet) {}
	virtual void handlePacket3Login(Packet3Login::const_ptr const& packet) {}
	virtual void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet) {}
	virtual void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet) {}
	virtual void handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet) {}
	virtual void handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet) {}
};