#pragma once

#include "Packet.h"

class Packet16GameOver :
	public Packet
{
private:
	const static uint16_t mId = 16;

	Packet16GameOver(Packet16GameOver& that);
	Packet16GameOver& operator=(Packet16GameOver& tmp);

protected:
	mutable uint16_t player1ID;
	mutable uint16_t player2ID;
	mutable float player1Score;
	mutable float player2Score;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet16GameOver();		// Prototype constructor
	Packet16GameOver(char const* data, uint16_t length);
	Packet16GameOver(unsigned short player1ID, unsigned short player2ID,
		float player1Score, float player2Score);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned short getPlayer1ID() const;
	unsigned short getPlayer2ID() const;
	float getPlayer1Score() const;
	float getPlayer2Score() const;
};