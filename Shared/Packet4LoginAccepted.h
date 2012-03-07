#pragma once

#include "Packet.h"

class Packet4LoginAccepted :
	public Packet
{
private:
	const static uint16_t mId = 4;

	Packet4LoginAccepted(Packet4LoginAccepted& that);
	Packet4LoginAccepted& operator=(Packet4LoginAccepted& tmp);

protected:
	mutable uint16_t* playerID;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet4LoginAccepted();		// Prototype constructor
	Packet4LoginAccepted(char const* data, uint16_t length);
	Packet4LoginAccepted(unsigned int playerID);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned int getPlayerID() const;
};