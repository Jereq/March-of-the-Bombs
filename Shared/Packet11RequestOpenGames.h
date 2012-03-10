#pragma once

#include "Packet.h"

class Packet11RequestOpenGames :
	public Packet
{
private:
	const static uint16_t mId = 11;

	Packet11RequestOpenGames(Packet11RequestOpenGames& that);
	Packet11RequestOpenGames& operator=(Packet11RequestOpenGames& tmp);

protected:
	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet11RequestOpenGames();
	Packet11RequestOpenGames(char const* data, uint16_t length);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;
};