#include "Packet10PlayerReady.h"

#include "Pack.h"

void Packet10PlayerReady::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();
	util::pack(&playerID, 1, &packedData[OFFSET_DATA]);

	packed = true;
}

void Packet10PlayerReady::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	util::unpack(&playerID, 1, &packedData[OFFSET_DATA]);

	unpacked = true;
}

Packet10PlayerReady::Packet10PlayerReady()
	: Packet(mId)
{
}

Packet10PlayerReady::Packet10PlayerReady(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet10PlayerReady::Packet10PlayerReady(unsigned short playerID)
	: Packet(mId), playerID(playerID)
{
	dataLength = OFFSET_DATA + sizeof(uint16_t);
	unpacked = true;
}

Packet::ptr Packet10PlayerReady::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet10PlayerReady(data, length));
}

unsigned short Packet10PlayerReady::getPlayerID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return playerID;
}