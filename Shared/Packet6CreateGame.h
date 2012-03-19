#pragma once

#include "Packet.h"

class Packet6CreateGame :
	public Packet
{
private:
	const static uint16_t mId = 6;
	const static size_t MAX_MAP_NAME_LENGTH = 256;

	Packet6CreateGame(Packet6CreateGame& that);
	Packet6CreateGame& operator=(Packet6CreateGame& tmp);

protected:
	mutable std::string mapName;
	mutable uint16_t winLimit;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet6CreateGame();		// Prototype constructor
	Packet6CreateGame(char const* data, uint16_t length);
	Packet6CreateGame(std::string const& mapName, unsigned short winLimit);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	std::string const& getMapName() const;
	unsigned short getWinLimit() const;
};