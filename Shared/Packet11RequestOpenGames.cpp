#include "Packet11RequestOpenGames.h"

#include "Pack.h"

void Packet11RequestOpenGames::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();

	packed = true;
}

void Packet11RequestOpenGames::unpack() const
{
	unpacked = true;
}

Packet11RequestOpenGames::Packet11RequestOpenGames()
	: Packet(mId)
{
	dataLength = OFFSET_DATA;
	unpacked = true;
}

Packet11RequestOpenGames::Packet11RequestOpenGames(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
	unpacked = true;
}

Packet::ptr Packet11RequestOpenGames::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet11RequestOpenGames(data, length));
}