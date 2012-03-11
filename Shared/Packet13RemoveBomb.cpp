#include "Packet13RemoveBomb.h"

#include "Pack.h"

void Packet13RemoveBomb::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::pack(&playerID, 1, dataP);
	dataP += util::pack(&entityID, 1, dataP);
	dataP += util::pack(&explode, 1, dataP);

	packed = true;
}

void Packet13RemoveBomb::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::unpack(&playerID, 1, dataP);
	dataP += util::unpack(&entityID, 1, dataP);
	dataP += util::unpack(&explode, 1, dataP);

	unpacked = true;
}

Packet13RemoveBomb::Packet13RemoveBomb()
	: Packet(mId)
{
}

Packet13RemoveBomb::Packet13RemoveBomb(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet13RemoveBomb::Packet13RemoveBomb(unsigned short playerID, unsigned short entityID, bool explode)
	: Packet(mId), playerID(playerID), entityID(entityID), explode(explode)
{
	dataLength = OFFSET_DATA +
		2 * sizeof(uint16_t) +
		sizeof(bool);
	unpacked = true;
}

Packet::ptr Packet13RemoveBomb::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet13RemoveBomb(data, length));
}

unsigned short Packet13RemoveBomb::getPlayerID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return playerID;
}

unsigned short Packet13RemoveBomb::getEntityID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return entityID;
}

bool Packet13RemoveBomb::getExplode() const
{
	if (!unpacked)
	{
		unpack();
	}

	return explode;
}