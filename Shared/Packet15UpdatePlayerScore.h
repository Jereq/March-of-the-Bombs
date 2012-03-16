#pragma once

#include "Packet.h"

class Packet15UpdatePlayerScore :
	public Packet
{
private:
	const static uint16_t mId = 15;

	Packet15UpdatePlayerScore(Packet15UpdatePlayerScore& that);
	Packet15UpdatePlayerScore& operator=(Packet15UpdatePlayerScore& tmp);

protected:
	mutable uint16_t playerID;
	mutable float newScore;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet15UpdatePlayerScore();		// Prototype constructor
	Packet15UpdatePlayerScore(char const* data, uint16_t length);
	Packet15UpdatePlayerScore(unsigned short playerID, float newScore);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned int getPlayerID() const;
	float getNewScore() const;
};