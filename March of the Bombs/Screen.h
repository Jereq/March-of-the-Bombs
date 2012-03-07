#pragma once

#include <boost\shared_ptr.hpp>							//include a boost smartpointer
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

class Screen
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
};