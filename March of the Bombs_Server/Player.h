#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

#include <Packet.h>

class Player
{
public:
	typedef boost::shared_ptr<Player> ptr;

	virtual ~Player() {};
	virtual void deliver(Packet::const_ptr packet) = 0;
};