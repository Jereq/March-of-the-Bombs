#pragma once

#include "Packet.h"

class Packet10PlayerReady :
	public Packet
{
private:
	const static uint16_t mId = 10;

	Packet10PlayerReady(Packet10PlayerReady& that);
	Packet10PlayerReady& operator=(Packet10PlayerReady& tmp);

protected:
	mutable uint16_t playerID;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet10PlayerReady();		// Prototype constructor
	Packet10PlayerReady(char const* data, uint16_t length);
	Packet10PlayerReady(unsigned short playerID);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned short getPlayerID() const;
};