#include "Packet4LoginAccepted.h"

#include "InvalidSizeException.h"
#include "Pack.h"

void Packet4LoginAccepted::pack() const
{
	return;	// If it has data, then it is already packed.
}

void Packet4LoginAccepted::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if (!checkHeader())
	{
		return;
	}

	playerID = reinterpret_cast<uint16_t*>(&packedData[OFFSET_DATA]);

	unpacked = true;
}

Packet4LoginAccepted::Packet4LoginAccepted()
	: Packet(mId), playerID(NULL)
{
}

Packet4LoginAccepted::Packet4LoginAccepted(char const* data, uint16_t length)
	: Packet(mId, data, length), playerID(NULL)
{
}

Packet4LoginAccepted::Packet4LoginAccepted(unsigned int playerID)
	: Packet(mId)
{
	if (playerID > UINT16_MAX)
	{
		throw InvalidSizeException("Player ID to great", Packet::ptr());
	}

	dataLength = sizeof(uint16_t) + OFFSET_DATA;
	packedData = new char[dataLength];

	packHeader();

	this->playerID = reinterpret_cast<uint16_t*>(&packedData[OFFSET_DATA]);

	uint16_t tId = playerID;
	util::pack(&tId, 1, &packedData[OFFSET_DATA]);

	packed = true;
	unpacked = true;
}

Packet::ptr Packet4LoginAccepted::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet4LoginAccepted(data, length));
}

unsigned int Packet4LoginAccepted::getPlayerID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return ntohs(*playerID);
}