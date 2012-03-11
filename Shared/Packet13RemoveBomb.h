#pragma once

#include "Packet.h"

class Packet13RemoveBomb :
	public Packet
{
private:
	const static uint16_t mId = 13;

	Packet13RemoveBomb(Packet13RemoveBomb& that);
	Packet13RemoveBomb& operator=(Packet13RemoveBomb& tmp);

protected:
	mutable uint16_t playerID;
	mutable uint16_t entityID;
	mutable bool explode;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet13RemoveBomb();		// Prototype constructor
	Packet13RemoveBomb(char const* data, uint16_t length);
	Packet13RemoveBomb(unsigned short playerID, unsigned short entityID, bool explode);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned short getPlayerID() const;
	unsigned short getEntityID() const;
	bool getExplode() const;
};