#pragma once

#include "Packet.h"

class Packet7JoinGame :
	public Packet
{
private:
	const static uint16_t mId = 7;

	Packet7JoinGame(Packet7JoinGame& that);
	Packet7JoinGame& operator=(Packet7JoinGame& tmp);

protected:
	mutable uint16_t* gameID;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet7JoinGame();		// Prototype constructor
	Packet7JoinGame(char const* data, uint16_t length);
	Packet7JoinGame(unsigned short gameID);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned int getGameID() const;
};