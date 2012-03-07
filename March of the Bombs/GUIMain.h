#pragma once

#include "Screen.h"
#include "Graphics.h"

#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>
#include <Packet4LoginAccepted.h>

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
};